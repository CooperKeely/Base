// Open and closing window
WM_Window wm_open_window(WM_Context* ctx, F32 width, F32 height, Str8 window_name){
	ctx->window_size = Vec2_F32(width, height);	
	ctx->title = window_name;

	WM_Window w = {0};
	return w;
}

void wm_close(WM_Context* ctx){
}


WM_Context* wm_init(Arena* arena, WM_ConfigFlag flags){
#if defined(OS_LINUX)
	wm_should_use_wayland();
#endif
	// os must be inialized before using os gfx 
	Assert(!MemoryIsZeroStruct(&os_state));
	
	WM_Context* ctx = ArenaPushStructZero(arena, WM_Context);
	ctx->flags = flags;
	return ctx; 
}

