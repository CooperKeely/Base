
///////////////////////////////////////////////////////////////////////////////
/// cjk: Window manager core api

WM_Display wm_init(Arena* arena, WM_ConfigFlag flags)
{
	WM_WaylandDisplay* disp = ArenaPushStructZero(arena, WM_WaylandDisplay);
	Assert(disp);

	disp->wl_display = wl_display_connect(NULL);
	Assert(disp->wl_display);

	if(disp->wl_display){
		LogCtxDebug(log_way, "Connected to display\n");
	}else{
		LogCtxError(log_way,"Failed to connect to display");
		InvalidPath;	
	}

	// bind to global wayland interfaces 
	disp->wl_registry = wl_display_get_registry(disp->wl_display);
	Assert(disp->wl_registry);

	wl_registry_add_listener(disp->wl_registry, &g_registry_listener, disp);
	wl_display_roundtrip(disp->wl_display);

	Assert(disp->wl_compositor);
	Assert(disp->xdg_wm_base);
	
	WM_Display ret_handle = Handle_Pack(WM_Display, disp);
	return ret_handle;
}

void wm_close(WM_Display disp)
{

}



/// cjk: Window Manger window functions
WM_Window wm_window_create(WM_Display disp, Arena* arena, F32 width, F32 height, Str8 window_name)
{
	WM_WaylandDisplay* wl_disp = Handle_Unpack(WM_WaylandDisplay*, disp);	
	WM_WaylandWindow* wl_win = ArenaPushStructZero(arena, WM_WaylandWindow);

	Assert(wl_win);
	Assert(wl_disp);	

	wl_win->height = height;
	wl_win->width = width;
	wl_win->name = window_name;	
	
	// Add to the list of displays
	wl_win->parent_display = wl_disp;
	DLLPushFront(wl_disp->window_head, wl_disp->window_tail, wl_win);

	// create a wayland surface
	wl_win->wl_surface = wl_compositor_create_surface(wl_disp->wl_compositor);
	Assert(wl_win->wl_surface);

	// assign a XDG role
	wl_win->xdg_surface = xdg_wm_base_get_xdg_surface(wl_disp->xdg_wm_base, wl_win->wl_surface);
	Assert(wl_win->xdg_surface);

	xdg_surface_add_listener(wl_win->xdg_surface, &g_xdg_surface_listener, wl_disp);
	wl_win->xdg_toplevel = xdg_surface_get_toplevel(wl_win->xdg_surface);
	Assert(wl_win->xdg_toplevel);
	
	// set the title
	xdg_toplevel_set_title(wl_win->xdg_toplevel, str8_to_cstring(arena, window_name));
	wl_win->name = window_name;

	wl_surface_commit(wl_win->wl_surface);
	
	WM_Window ret_handle = Handle_Pack(WM_Window, wl_win);
	return ret_handle;
}

void wm_window_close(WM_Window window)
{
	WM_WaylandWindow* wl_win = Handle_Unpack(WM_WaylandWindow*, window); 
	WM_WaylandDisplay* wl_disp = wl_win->parent_display;
	Assert(wl_win);

	if(wl_win->xdg_toplevel) xdg_toplevel_destroy(wl_win->xdg_toplevel);
	if(wl_win->xdg_surface) xdg_surface_destroy(wl_win->xdg_surface);
	if(wl_win->wl_surface) wl_surface_destroy(wl_win->wl_surface);
	if(wl_disp->wl_registry) wl_registry_destroy(wl_disp->wl_registry);
	if(wl_disp->wl_display) wl_display_disconnect(wl_disp->wl_display);
}


void wm_window_set_title(WM_Window window, Str8 window_name);
void wm_window_is_focused(WM_Window window);
void wm_window_focus(WM_Window window);
void wm_window_is_fullscreen(WM_Window window);
void wm_window_set_fullscreen(WM_Window window);
void wm_window_is_maximized(WM_Window window);
void wm_window_set_maximized(WM_Window window);
void wm_window_is_minimized(WM_Window window);
void wm_window_set_minimized(WM_Window window);
void wm_window_bring_to_front(WM_Window window);
void wm_window_set_monitor(WM_Window window, WM_Monitor monitor);
F32 wm_window_get_dpi(WM_Window window);

///////////////////////////////////////////////////////////////////////////////
/// cjk: Window manager event api


WM_Event* wm_event_get_list(WM_Window window, Arena* arena)
{
	WM_WaylandWindow* wl_win = Handle_Unpack(WM_WaylandWindow*, window);
	Assert(wl_win);	
	
	// set the current frame arena so all events get allocated to it
	wl_win->current_frame_arena = arena;

	Assert(wl_win->parent_display && wl_win->parent_display->wl_display);
	wl_display_dispatch_pending(wl_win->parent_display->wl_display);	
	
	WM_Event* ret_head = wl_win->event_head;
	
	// clear the persistent linked list	
	wl_win->event_head = NULL;
	wl_win->current_frame_arena = NULL;

	return ret_head; 
}

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland Event Listener Implementations
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland Display Discovery

static void registry_handle_global(void* data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version)
{
	WM_WaylandDisplay* wl_disp = (WM_WaylandDisplay*) data;

	str8_printf(stdout,"interface : '%s', version: %d, name: %d\n", interface, version, name);

	if (strcmp(interface, wl_compositor_interface.name) == 0) {
        	wl_disp->wl_compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 4);
    	}
	else if(strcmp(interface, xdg_wm_base_interface.name) == 0){
		wl_disp->xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);	
		xdg_wm_base_add_listener(wl_disp->xdg_wm_base, &g_xdg_wm_base_listener, wl_disp);
	}
}

static void registry_handle_global_remove(void* data, struct wl_registry* registry, uint32_t name)
{}

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland XDG Window Management

// xdg_wm_base
static void xdg_wm_base_handle_ping(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial)
{
	(void)data;
    	xdg_wm_base_pong(xdg_wm_base, serial);
}

// xdg_surface
static void xdg_surface_handle_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial)
{
	(void) data;
	xdg_surface_ack_configure(xdg_surface, serial);
}

// xdg_toplevel
static void xdg_toplevel_handle_configure(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height, struct wl_array* states)
{}
static void xdg_toplevel_handle_close(void* data, struct xdg_toplevel* xdg_toplevel)
{}
static void xdg_toplevel_handle_configure_bounds(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height)
{}
static void xdg_toplevel_handle_wm_capabilities(void* data, struct xdg_toplevel* xdg_toplevel, struct wl_array* capabilities)
{}

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland Keyboard and Mouse Listeners

// wl_seat
static void seat_handle_capabilities(void* data, struct wl_seat* seat, uint32_t caps)
{NotImplemented;}
static void seat_handle_name(void* data, struct wl_seat* seat, const char* name)
{NotImplemented;}

// wl_keyboard
static void keyboard_handle_keymap(void* data, struct wl_keyboard* kb, uint32_t format, int32_t fd, uint32_t size)
{NotImplemented;}
static void keyboard_handle_enter(void* data, struct wl_keyboard* kb, uint32_t serial, struct wl_surface* surface, struct wl_array* keys)
{NotImplemented;}
static void keyboard_handle_leave(void* data, struct wl_keyboard* kb, uint32_t serial, struct wl_surface* surface)
{NotImplemented;}
static void keyboard_handle_key(void* data, struct wl_keyboard* kb, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
{NotImplemented;}
static void keyboard_handle_modifiers(void* data, struct wl_keyboard* kb, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group)
{NotImplemented;}
static void keyboard_handle_repeat_info(void* data, struct wl_keyboard* kb, int32_t rate, int32_t delay)
{NotImplemented;}

// wl_pointer
static void pointer_handle_enter(void* data, struct wl_pointer* ptr, uint32_t serial, struct wl_surface* surface, wl_fixed_t sx, wl_fixed_t sy)
{NotImplemented;}
static void pointer_handle_leave(void* data, struct wl_pointer* ptr, uint32_t serial, struct wl_surface* surface)
{NotImplemented;}
static void pointer_handle_motion(void* data, struct wl_pointer* ptr, uint32_t time, wl_fixed_t sx, wl_fixed_t sy)
{NotImplemented;}
static void pointer_handle_button(void* data, struct wl_pointer* ptr, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
{NotImplemented;}
static void pointer_handle_axis(void* data, struct wl_pointer* ptr, uint32_t time, uint32_t axis, wl_fixed_t value)
{NotImplemented;}
static void pointer_handle_frame(void* data, struct wl_pointer* ptr)
{NotImplemented;}
static void pointer_handle_axis_source(void* data, struct wl_pointer* ptr, uint32_t axis_source)
{NotImplemented;}
static void pointer_handle_axis_stop(void* data, struct wl_pointer* ptr, uint32_t time, uint32_t axis)
{NotImplemented;}
static void pointer_handle_axis_discrete(void* data, struct wl_pointer* ptr, uint32_t axis, int32_t discrete)
{NotImplemented;}

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland Rendering & Sync listeners

// wl_callback (Frame Pacing / VSync)
static void frame_handle_done(void* data, struct wl_callback* callback, uint32_t callback_data)
{NotImplemented;}

// wl_buffer (Software Rendering / Release notification)
static void buffer_handle_release(void* data, struct wl_buffer* buffer)
{NotImplemented;}

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland Monitor Listeners

static void output_handle_geometry(void* data, struct wl_output* output, int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const char* make, const char* model, int32_t transform)
{NotImplemented;}
static void output_handle_mode(void* data, struct wl_output* output, uint32_t flags, int32_t width, int32_t height, int32_t refresh)
{NotImplemented;}
static void output_handle_done(void* data, struct wl_output* output)
{NotImplemented;}
static void output_handle_scale(void* data, struct wl_output* output, int32_t factor)
{NotImplemented;}
static void output_handle_name(void* data, struct wl_output* output, const char* name)
{NotImplemented;}
static void output_handle_description(void* data, struct wl_output* output, const char* description)
{NotImplemented;}

