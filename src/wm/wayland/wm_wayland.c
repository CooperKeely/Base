
///////////////////////////////////////////////////////////////////////////////
/// cjk: Window manager core api



void wm_create_connection(WM_ConfigFlag flags)
{
	WM_WaylandDisplay* disp = &g_wm_wayland_display;

	disp->wl_display = wl_display_connect(NULL);
	Assert(disp->wl_display);

	// bind to global wayland interfaces 
	disp->wl_registry = wl_display_get_registry(disp->wl_display);
	Assert(disp->wl_registry);

	wl_registry_add_listener(disp->wl_registry, &g_registry_listener, disp);
	wl_display_roundtrip(disp->wl_display);

	Assert(disp->wl_compositor);
	Assert(disp->xdg_wm_base);
	
	return ret_handle;
}

void wm_close_connection(){

}



/// cjk: Window Manger window functions
WM_Window wm_create_window(Arena* arena, F32 width, F32 height, Str8 window_name)
{
	WM_WaylandDisplay* disp = &g_wm_wayland_display;
	WM_WaylandWindow* win = ArenaPushStructZero(arena, WM_WaylandWindow);

	Assert(win);
	Assert(disp);	

	win->height = height;
	win->width = width;
	win->name = window_name;	
	
	// Add window to the list of windows attached to window manger connection
	DLLPushFront(disp->window_head, disp->window_tail, win);

	// create a wayland surface
	win->wl_surface = wl_compositor_create_surface(disp->wl_compositor);
	Assert(win->wl_surface);

	// assign a XDG role
	win->xdg_surface = xdg_wm_base_get_xdg_surface(disp->xdg_wm_base, win->wl_surface);
	Assert(win->xdg_surface);

	xdg_surface_add_listener(win->xdg_surface, &g_xdg_surface_listener, disp);
	win->xdg_toplevel = xdg_surface_get_toplevel(win->xdg_surface);
	Assert(win->xdg_toplevel);
	
	// set the title
	xdg_toplevel_set_title(win->xdg_toplevel, str8_to_cstring(arena, window_name));
	win->name = window_name;

	wl_surface_commit(win->wl_surface);
	
	WM_Window ret_handle = Handle_Pack(WM_Window, win);
	return ret_handle;
}

void wm_close_window(WM_Window window)
{
	WM_WaylandDisplay* disp = &g_wm_wayland_display;
	WM_WaylandWindow* win = Handle_Unpack(WM_WaylandWindow*, window); 
	Assert(win);

	if(win->xdg_toplevel) xdg_toplevel_destroy(win->xdg_toplevel);
	if(win->xdg_surface) xdg_surface_destroy(win->xdg_surface);
	if(win->wl_surface) wl_surface_destroy(win->wl_surface);
	if(disp->wl_registry) wl_registry_destroy(disp->wl_registry);
	if(disp->wl_display) wl_display_disconnect(disp->wl_display);
}

WM_NativeHandles wm_export_native_handles(WM_Window window){

	WM_NativeHandles handles = {0};
	WM_WaylandWindow* way_win = Handle_Unpack(WM_WaylandWindow*, window);

	Assert(way_win);

	handles.type = GFX_NativeWindowType_Wayland;
	handles.wayland.display = (void*) g_wm_wayland_display.wl_display;
	handles.wayland.surface =  (void*) way_win->wl_surface;

	return handles;
}


void wm_begin_frame(WM_Window window){

}

void wm_end_frame(WM_Window window){

}



// cjk: window functions
void wm_set_window_title(WM_Window window, Str8 window_name);
void wm_is_window_focused(WM_Window window);
void wm_set_window_focus(WM_Window window);
void wm_is_window_fullscreen(WM_Window window);
void wm_set_window_fullscreen(WM_Window window);
void wm_is_window_maximized(WM_Window window);
void wm_set_window_maximized(WM_Window window);
void wm_is_window_minimized(WM_Window window);
void wm_set_window_minimized(WM_Window window);
void wm_bring_window_to_front(WM_Window window);
void wm_set_window_monitor(WM_Window window, WM_Monitor monitor);

///////////////////////////////////////////////////////////////////////////////
/// cjk: Window manager event api


WM_Event* wm_get_event_list(WM_Window window, Arena* arena)
{
	WM_WaylandDisplay* disp = &g_wm_wayland_display;
	WM_WaylandWindow* win = Handle_Unpack(WM_WaylandWindow*, window);
	Assert(win);	
	
	// set the current frame arena so all events get allocated to it
	win->current_frame_arena = arena;

	wl_display_dispatch_pending(disp->wl_display);	
	
	WM_Event* ret_head = win->event_head;
	
	// clear the persistent linked list	
	win->event_head = NULL;
	win->current_frame_arena = NULL;

	return ret_head; 
}

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland Event Listener Implementations
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland Display Discovery

static void registry_handle_global(void* data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version)
{
	WM_WaylandDisplay* disp = &g_wm_wayland_display;
	str8_printf(stdout,"interface : '%s', version: %d, name: %d\n", interface, version, name);

	if (strcmp(interface, wl_compositor_interface.name) == 0) {
        	disp->wl_compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 4);
    	}
	else if(strcmp(interface, xdg_wm_base_interface.name) == 0){
		disp->xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);	
		xdg_wm_base_add_listener(disp->xdg_wm_base, &g_xdg_wm_base_listener, disp);
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
{
	WM_WaylandWindow* window = (WM_WaylandWindow*) data; 
	if(width > 0 && height > 0){
		// TODO: update window dimenstions queue a resize event

	}
}
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

