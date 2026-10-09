static LOG_Context log_way = (LOG_Context){
	.fd 		= LOG_FD_STDERR,
	.level 		= LOG_Level_All,
	.log_flags	= LOG_Option_TimeStamp | LOG_Option_Abort,
	.category	= { sizeof("WAYLAND") - 1, "WAYLAND" },
};

// (cjk): listener structs 

static void registry_handle_global(void* data, struct wl_registry *registry, U32 name, const char* interface, U32 version);
static void registry_handle_global_remove(void* data, struct wl_registry* registry, U32 name);
static void xdg_surface_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial);
static void xdg_wm_base_ping(void* data, struct xdg_wm_base *xdg_wm_base, uint32_t serial);


static const struct wl_registry_listener registry_listener = {
	.global = registry_handle_global,
	.global_remove = registry_handle_global_remove,
};

static const struct xdg_surface_listener xdg_surface_listener = {
	.configure = xdg_surface_configure,
};

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
	.ping = xdg_wm_base_ping,
};

// (cjk): forward declared function definitions
static void registry_handle_global(void* data, struct wl_registry *registry, U32 name, const char* interface, U32 version){
	OS_GFX_PlatformContext* plat_ctx = (OS_GFX_PlatformContext*) data;
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
	OS_GFX_PlatformContext* platform_context = (OS_GFX_PlatformContext*) data;
	
	xdg_surface_ack_configure(xdg_surface, serial);
}

static void xdg_wm_base_ping(void* data, struct xdg_wm_base *xdg_wm_base, uint32_t serial){
	xdg_wm_base_pong(xdg_wm_base, serial);	
}


// (cjk): egl initializer
void os_gfx_init_egl(OS_GFX_PlatformContext* ctx, U32 width, U32 height){
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

	gl_loader_load_opengl_extensions();	
}

// (cjk): api implementations
void os_gfx_init_platform(Arena* arena, OS_GFX_Context* ctx){
	Assert(ctx);

	// initialize platform context
	OS_GFX_PlatformContext* plat_ctx = ArenaPushStruct(arena, OS_GFX_PlatformContext);
	Assert(plat_ctx);

	plat_ctx->wl_display = wl_display_connect(NULL);
	Assert(plat_ctx->wl_display);

	if(plat_ctx->wl_display){
		LogCtxDebug(log_way, "Connected to display\n");
	}else{
		LogCtxError(log_way,"Failed to connect to display");
		InvalidPath;	
	}

	plat_ctx->wl_registry = wl_display_get_registry(plat_ctx->wl_display);
	Assert(plat_ctx->wl_registry);

	wl_registry_add_listener(plat_ctx->wl_registry, &registry_listener, plat_ctx);
	wl_display_roundtrip(plat_ctx->wl_display);
	Assert(plat_ctx->wl_compositor);
	Assert(plat_ctx->xdg_wm_base);

	plat_ctx->wl_surface = wl_compositor_create_surface(plat_ctx->wl_compositor);
	Assert(plat_ctx->wl_surface);

	plat_ctx->xdg_surface = xdg_wm_base_get_xdg_surface(plat_ctx->xdg_wm_base, plat_ctx->wl_surface);
	Assert(plat_ctx->xdg_surface);

	xdg_surface_add_listener(plat_ctx->xdg_surface, &xdg_surface_listener, plat_ctx);
	plat_ctx->xdg_toplevel = xdg_surface_get_toplevel(plat_ctx->xdg_surface);
	Assert(plat_ctx->xdg_toplevel);

	xdg_toplevel_set_title(plat_ctx->xdg_toplevel, str8_to_cstring(arena, ctx->title));
	wl_surface_commit(plat_ctx->wl_surface);
	wl_display_roundtrip(plat_ctx->wl_display);

	os_gfx_init_egl(plat_ctx, 100, 100);

	// Clear the buffer and swap to commit the first frame to the compositor
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(plat_ctx->egl_display, plat_ctx->egl_surface);

	ctx->platform_context = plat_ctx;
}

void os_gfx_close_platform(OS_GFX_Context* ctx){
	struct wl_display* display = ctx->platform_context->wl_display;
	Assert(display);
	wl_display_disconnect(display);
}

