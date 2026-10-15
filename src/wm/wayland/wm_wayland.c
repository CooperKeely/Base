///////////////////////////////////////
/// cjk: Register Listeners

static void registry_handle_global(void* data, struct wl_registry *registry, U32 name, const char* interface, U32 version){
	WM_PlatformContext* plat_ctx = (WM_PlatformContext*) data;
	str8_printf(stdout,"interface : '%s', version: %d, name: %d\n", interface, version, name);

	if (strcmp(interface, wl_compositor_interface.name) == 0) {
        	plat_ctx->wl_compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 4);
    	}
	else if(strcmp(interface, xdg_wm_base_interface.name) == 0){
		plat_ctx->xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);	
		xdg_wm_base_add_listener(plat_ctx->xdg_wm_base, &xdg_wm_base_listener, plat_ctx);
	}

}

static void registry_handle_global_remove(void* data, struct wl_registry* registry, U32 name) { }

static void xdg_surface_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial){
	WM_PlatformContext* platform_context = (WM_PlatformContext*) data;
	
	xdg_surface_ack_configure(xdg_surface, serial);
}

static void xdg_wm_base_ping(void* data, struct xdg_wm_base *xdg_wm_base, uint32_t serial){
	xdg_wm_base_pong(xdg_wm_base, serial);	
}


// (cjk): egl initializer
/* TODO: move to rendering API
internal void wm_init_egl(WM_PlatformContext* ctx, U32 width, U32 height){
	EGLint major;
	EGLint minor;
	EGLConfig config;
	EGLint num_config;

	EGLint ctx_attributes[] = { EGL_NONE };
	EGLint attributes[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_NONE
	};

	EGLBoolean err;

	ctx->egl_display = eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, ctx->wl_display, NULL);
	if(ctx->egl_display == EGL_NO_DISPLAY){ LogCtxError(log_way, "Failed to get EGL display"); }
	
	err = eglInitialize(ctx->egl_display, &major, &minor);
	if(!err) { LogCtxError(log_way, "Failed to initialize EGL"); }

	err = eglChooseConfig(ctx->egl_display, attributes, &config, 1, &num_config);
	if(!err || num_config < 1){ LogCtxError(log_way, "Failed to choose EGL config"); }

	err = eglBindAPI(EGL_OPENGL_API);
	if(!err) { LogCtxError(log_way, "Failed to bind EGL OpenGL API"); }

	ctx->egl_context = eglCreateContext(ctx->egl_display, config, EGL_NO_CONTEXT, ctx_attributes);
	if(ctx->egl_context == EGL_NO_CONTEXT) { LogCtxError(log_way, "Failed to create EGL context"); }

	ctx->egl_window = wl_egl_window_create(ctx->wl_surface, width, height);
	if(!ctx->egl_window) { LogCtxError(log_way, "Failed to create EGL window"); }

	ctx->egl_surface = eglCreatePlatformWindowSurface(ctx->egl_display, config, ctx->egl_window, NULL);
	if(!ctx->egl_surface) { LogCtxError(log_way, "Failed to create EGL surface"); }

	err = eglMakeCurrent(ctx->egl_display, ctx->egl_surface, ctx->egl_surface, ctx->egl_context);
	if(!err) { LogCtxError(log_way, "Failed to make EGL context current"); }

	gl_loader_init((GL_LoaderProc)eglGetProcAddress);	
}*/

WM_Display wm_init(Arena* arena, WM_ConfigFlag flags)
{
	WM_WaylandDisplay* disp = ArenaPushStructZero(arena, WM_Context);
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

	wl_registry_add_listener(disp->wl_registry, &registry_listener, disp);
	wl_display_roundtrip(disp->wl_display);

	Assert(disp->wl_compositor);
	Assert(disp->xdg_wm_base);
	
	WM_Display ret_handle = Handle_Pack(WM_Display, disp);
	return ret_handle;
}

void wm_close(WM_Context* ctx)
{

}



/// cjk: Window Manger window functions
WM_Window wm_window_create(WM_Display disp, Arena arena, F32 width, F32 height, Str8 window_name)
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
	DLLPushFront(wl_disp->first, wl_disp->last, wl_win);

	// create a wayland surface
	wl_win->wl_surface = wl_compositor_create_surface(wl_disp->wl_compositor);
	Assert(wl_win->wl_surface);

	// assign a XDG role
	wl_win->xdg_surface = xdg_wm_base_get_xdg_surface(wl_disp->xdg_wm_base, wl_win->wl_surface);
	Assert(wl_win->xdg_surface);

	xdg_surface_add_listener(wl_win->xdg_surface, &xdg_surface_listener, wl_disp);
	wl_win->xdg_toplevel = xdg_surface_get_toplevel(wl_win->xdg_surface);
	Assert(wl_win->xdg_toplevel);
	
	// set the title
	xdg_toplevel_set_title(wl_win->xdg_toplevel, str8_to_cstring(arena, window_name));
	wm_ctx->title = window_name;

	wl_surface_commit(wl_win->wl_surface);
	
	WM_Window ret_handle = Handle_Pack(WM_Window, wl_win);
	return ret;

	/* TODO: move to rendering api
	 wm_init_egl(plat_ctx, 100, 100);

	// Clear the buffer and swap to commit the first frame to the compositor
	glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(plat_ctx->egl_display, plat_ctx->egl_surface);

	ctx->platform_context = plat_ctx;*/
}

void wm_window_close(WM_Window window)
{
	WM_WaylandWindow* wl_win = Handle_Unpack(WM_WaylandWindow*, window); 
	Assert(wl_win);

	if(wl_win->xdg_toplevel) xdg_toplevel_destroy(wl_win->xdg_toplevel);
	if(wl_win->xdg_surface) xdg_surface_destroy(wl_win->xdg_surface);
	if(wl_win->wl_surface) wl_surface_destroy(wl_win->wl_surface);
	if(wl_win->wl_registry) wl_registry_destroy(wl_win->wl_surface);
	if(wl_win->wl_display) wl_display_disconnect(wl_win->wl_display);
}


/// cjk: API Implemented Per OS
WM_Display wm_init(Arena* arena, WM_ConfigFlag flags);
void wm_close(WM_Display disp);

WM_Window wm_window_create(WM_Display disp, Arena arena, F32 width, F32 height, Str8 window_name);
void wm_window_close(WM_Window window);

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

/// cjk: Event function
WM_Event wm_event_get_list(WM_Window window, Arena arena);


