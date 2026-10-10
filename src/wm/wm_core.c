// Open and closing window
OS_WIN_Context* os_win_init_window(Arena* arena, F32 x, F32 y, F32 width, F32 height, Str8 window_name){
	// os must be inialized before using os gfx 
	Assert(!MemoryIsZeroStruct(&os_state));
	
	// make sure all memory is set to zero
	OS_WIN_Context* ctx = ArenaPushStructZero(arena, OS_WIN_Context);

	ctx->position = Vec2_F32(x, y);	
	ctx->window_size = Vec2_F32(width, height);	
	ctx->title = window_name;

	os_win_init_platform(arena, ctx);

	return ctx;
}

void os_win_close_window(OS_WIN_Context* ctx){
	os_win_close_platform(ctx);
}


B32 wm_init(Arena* arena){
#if defined(OS_LINUX)
	wm_should_use_wayland();
#endif
}

