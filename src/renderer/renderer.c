void r_set_global_render_context(R_RenderContext* ctx){
	glb_r_context = ctx;
}

R_RenderContext* r_get_global_render_context(){
	return glb_r_context;
}

R_RenderCommand* r_push_render_command(R_RenderCommandType type){
	R_RenderContext* ctx = r_get_global_render_context();
	if(ctx && ctx->count < ctx->capacity){
		R_RenderCommand* cmd = &ctx->command_queue[ctx->count++];
		MemoryZeroStruct(cmd);
		cmd->type = type;
		return cmd;
	}
	InvalidPath;	
	return NULL;
}

R_RenderContext* r_init_render(Arena* arena, R_RenderingBackend type){
	R_RenderContext* ctx = ArenaPushStructZero(arena, R_RenderContext); 
	
	ctx->count = 0;
	ctx->capacity = R_MAX_RENDER_COMMANDS;
	ctx->command_queue = ArenaPushArray(arena, R_RenderCommand, ctx->capacity);
	ctx->backend = type;

	r_set_global_render_context(ctx);

	r_backend_init(ctx);

	return ctx;
}

void r_begin_frame(R_RenderTarget target){
	R_RenderContext* ctx = r_get_global_render_context();
	ctx->target = target;	
	
	ctx->count = 0; 

	r_backend_begin_frame(ctx, target);
}

void r_end_frame(){
	R_RenderContext* ctx = r_get_global_render_context();
	r_backend_submit_commands(ctx->command_queue, ctx->count);
	r_backend_end_frame(ctx);
}

// basic primitive drawing
void r_draw_line(Vec3F32 p0, Vec3F32 p1, ColorRGBA c){
	R_RenderCommand* cmd = r_push_render_command(R_RenderCommandType_Line);
	cmd->line.p0 = p0;
	cmd->line.p1 = p1;
	cmd->color = c;
	cmd->options = R_RenderCommandOption_None; 
}

void r_draw_triangle(Vec3F32 p0, Vec3F32 p1, Vec3F32 p2, ColorRGBA c){
	R_RenderCommand* cmd = r_push_render_command(R_RenderCommandType_Triangle);
	cmd->triangle.p0 = p0;
	cmd->triangle.p1 = p1;
	cmd->triangle.p2 = p2;
	cmd->color = c;
	cmd->options = R_RenderCommandOption_None;
}

void r_draw_quad(Vec3F32 p0, Vec3F32 p1, Vec3F32 p2, Vec3F32 p3, ColorRGBA c){
	R_RenderCommand* cmd = r_push_render_command(R_RenderCommandType_Quad);
	cmd->quad.p0 = p0;
	cmd->quad.p1 = p1;
	cmd->quad.p2 = p2;
	cmd->quad.p3 = p3;
	cmd->color = c;
	cmd->options = R_RenderCommandOption_None;
}

void r_draw_background(ColorRGBA c){
	R_RenderContext* ctx = r_get_global_render_context();
	R_RenderCommand* cmd = r_push_render_command(R_RenderCommandType_Quad);
	cmd->quad.p0 = Vec3_F32(F32Lit(0.0), F32Lit(0.0), F32Lit(0.0));
	cmd->quad.p1 = Vec3_F32(F32Lit(0.0), F32Lit(0.0), F32Lit(0.0));
	cmd->quad.p2 = Vec3_F32(ctx->target.width, F32Lit(0.0), F32Lit(0.0));
	cmd->quad.p3 = Vec3_F32(F32Lit(0.0), ctx->target.height, F32Lit(0.0));
	cmd->color = c;
	cmd->options = R_RenderCommandOption_None;
}
