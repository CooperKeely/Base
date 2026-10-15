/* TODO: move to rendering api
	 wm_init_egl(plat_ctx, 100, 100);

	// Clear the buffer and swap to commit the first frame to the compositor
	glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(plat_ctx->egl_display, plat_ctx->egl_surface);

	ctx->platform_context = plat_ctx;*/

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
