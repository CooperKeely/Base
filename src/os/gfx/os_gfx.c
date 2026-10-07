// Open and closing window
OS_GFX_Context* os_gfx_init_window(Arena* arena, F32 x, F32 y, F32 width, F32 height, Str8 window_name){
	// os must be inialized before using os gfx 
	Assert(!MemoryIsZeroStruct(&os_state));
	
	// make sure all memory is set to zero
	OS_GFX_Context* ctx = ArenaPushStructZero(arena, OS_GFX_Context);

	ctx->position = Vec2_F32(x, y);	
	ctx->window_size = Vec2_F32(width, height);	
	ctx->title = window_name;

	os_gfx_init_platform(arena, ctx);

	return ctx;
}

void os_gfx_close_window(OS_GFX_Context* ctx){
	os_gfx_close_platform(ctx);
}



