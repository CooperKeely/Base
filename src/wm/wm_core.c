// Open and closing window
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

// Open and closing window

WM_Window wm_create_window(WM_Context* wm_ctx, F32 width, F32 height, Str8 window_name){
	wm_ctx->window_size = Vec2_F32(width, height);	
	wm_ctx->title = window_name;

	WM_Window w = {0};
	return w;

}

void wm_close_window(WM_Context* ctx){

}


