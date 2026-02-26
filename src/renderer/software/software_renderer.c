
// This method uses Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm  
// https://groups.csail.mit.edu/graphics/classes/6.837/F02/lectures/6.837-7_Line.pdf
// https://zingl.github.io/Bresenham.pdf
void soft_r_draw_line(Vec2F32 p1, Vec2F32 p2, ColorRGBA color){
	S32 window_width = (S32) os_gfx_get_screen_width();
	S32 window_height = (S32) os_gfx_get_screen_height();

	S32 x0 = (S32) p1.x;
	S32 y0 = (S32) p1.y; 
	S32 x1 = (S32) p2.x; 
	S32 y1 = (S32) p2.y;

	S32 dx = abs(x1 - x0);
	S32 dy = -abs(y1 - y0);
	S32 sx = (x0 < x1)? 1 : -1;
	S32 sy = (y0 < y1)? 1 : -1;
	S32 err = dx+dy;
	S32 err_2;

	for(;;){
		if( 0 <= x0 && x0 < window_width && 0 <= y0 && y0 < window_height){
			os_gfx_paint_pixel(x0, y0, color); 
		}
		
		err_2 = 2 * err;

		if(err_2 >= dy){
			if(x0 == x1) break;
			err += dy;
			x0 += sx;
		}
		if(err_2 <= dx){
			if(y0 == y1) break;
			err += dx;
			y0 += sy;
		}
	}
}

void soft_r_draw_triangle(Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color){
	sr_draw_line(v1, v2, color);
	sr_draw_line(v2, v3, color);
	sr_draw_line(v3, v1, color);
} 

B32 soft_r_is_point_in_triangle(Vec2F32 point, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3){
	B32 result = BASE_FALSE;	
		 		
	return result;	
}

void soft_r_draw_filled_triangle(Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color){
	S32 window_width = (S32) os_gfx_get_screen_width();
	S32 window_height = (S32) os_gfx_get_screen_height();

	// get bounding box
	U32 x_min = Min(0, Min(v1.x, Min(v2.x, v3.x)));
	U32 y_min = Min(0, Min(v1.y, Min(v2.y, v3.y)));
	U32 x_max = Min(window_width, Max(v1.x, Max(v2.x, v3.x)));
	U32 y_max = Min(window_height, Max(v1.y, Max(v2.y, v3.y)));

	Rng1U32 x_rng = Rng1_U32(x_min, x_max);
	Rng1U32 y_rng = Rng1_U32(y_min, y_max);

	for EachInRange(y, y_rng){
		for EachInRange(x, x_rng){
			if(sr_is_point_in_triangle(Vec2_F32(x, y), v1, v2, v3)){
				os_gfx_paint_pixel(x, y, color);
			}
		}	
	}
}


// 2d primitive drawing
void soft_r_draw_rect(RectF32 rect, ColorRGBA color){
	S32 window_width = (S32) os_gfx_get_screen_width();
	S32 window_height = (S32) os_gfx_get_screen_height();

	U32 x1 = Max(0, (S32)rect.x);
	U32 y1 = Max(0, (S32)rect.y);
	U32 x2 = Min(window_width, (S32)(rect.x + rect.width));
	U32 y2 = Min(window_height, (S32)(rect.y + rect.height));

	if( x1 >= x2 || y1 >= y2 ) return;

	U32 rect_width = x2 - x1;
	for(U32 y = y1; y < y2; y ++){
		for (U32 x = x1; x < x2; x ++){
			os_gfx_paint_pixel(x, y, color);
		}
	}	

}

// camera projections
Vec2F32 soft_r_orthographic_projection(Vec3F32 vec, R_Camera camera);
Vec2F32 soft_r_perspective_projection(Vec3F32 vec, R_Camera camera);

// rendering api functions
void soft_r_init(R_RenderContext* ctx){}
void soft_r_begin_frame(R_RenderContext* ctx, R_RenderTarget target){}

void soft_r_submit_commands(R_RenderCommand* cmd, U64 count){
	for EachIndex(idx, count){
		R_RenderCommand* curr_cmd = cmd[idx];
		switch(curr_cmd.type){
			case R_RenderCommandType_Line:{
				soft_r_draw_line(curr_cmd->line.p0, 
			 				curr_cmd->line.p1, 
			 				curr_cmd->color );			
			}break;
			case R_RenderCommandType_Triangle:{}break;
			case R_RenderCommandType_Quad:{}break;
			default:{
				LogErr("Unknown render command");
			}break;
		}
	}
}

void soft_r_end_frame(R_RenderContext* ctx){}
void soft_r_close(R_RenderContext* ctx){}

/*
// This method uses Midpoint circle algorithm for quickly drawing a circle
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm 
void sr_draw_circle(Vec2F32 center, F32 radius, ColorRGBA color){
	Assert(radius > 0.0);

	S32 window_width = (S32) os_gfx_get_screen_width();
	S32 window_height = (S32) os_gfx_get_screen_height();

	S32 x0 = (S32) floorf(center.x);
	S32 y0 = (S32) floorf(center.y);
	S32 r  = (S32) radius;

	S32 x = r;
	S32 y = 0;
	S32 decision = 1 - r;

	#define LOCAL_MACRO_DRAWSPAN(py, x_start, x_end) do{\
			if((py) >= (0) && (py) <= (window_height)) {\
      				S32 left = Max(0, (x_start));\
				S32 right = Min((window_width-1),(x_end));\
				for(S32 px = left; px < right; px ++){\
					os_gfx_paint_pixel(px, py, color);\
				}\
			}\
		}while(0)

	while(x >= y){
		LOCAL_MACRO_DRAWSPAN(y0+y, x0-x, x0+x);
		LOCAL_MACRO_DRAWSPAN(y0-y, x0-x, x0+x);
		LOCAL_MACRO_DRAWSPAN(y0+x, x0-y, x0+y);
		LOCAL_MACRO_DRAWSPAN(y0-x, x0-y, x0+y);

		y++;
		if(decision <= 0){
			decision += 2 * y + 1;
		}else{
			x--;
			decision += 2 * (y - x) + 1;
		}
	}
	#undef Local_Macro_DRAWSPAN
}


void sr_draw_obj(FMT_OBJ_Object* obj_file, SR_ObjectRenderFlag flags){
	U32 width = os_gfx_get_screen_width();
	U32 height = os_gfx_get_screen_height();

	FMT_OBJ_LineArray* vertex_array = &obj_file->vertex_array;
	FMT_OBJ_LineArray* face_array = &obj_file->face_array;
	FMT_OBJ_LineArray* normal_array = &obj_file->normal_array;
	FMT_OBJ_LineArray* texture_array = &obj_file->texture_array;

	for EachIndex(face_idx, face_array->count){
		FMT_OBJ_Line* current_face = fmt_obj_line_array_get(face_array, face_idx);
		
		U32 v_idx1 = current_face->face.corner[0].v_idx-1;
		U32 v_idx2 = current_face->face.corner[1].v_idx-1;
		U32 v_idx3 = current_face->face.corner[2].v_idx-1;

		Vec4F32 v1 = fmt_obj_line_array_get(vertex_array, v_idx1)->v;
		Vec4F32 v2 = fmt_obj_line_array_get(vertex_array, v_idx2)->v;
		Vec4F32 v3 = fmt_obj_line_array_get(vertex_array, v_idx3)->v;
		
		Vec2F32 p1 = sr_viewport_transform(Vec3_F32(v1.x, v1.y, v1.z), width, height); 
		Vec2F32 p2 = sr_viewport_transform(Vec3_F32(v2.x, v2.y, v2.z), width, height); 
		Vec2F32 p3 = sr_viewport_transform(Vec3_F32(v3.x, v3.y, v3.z), width, height); 

		sr_draw_triangle(p1, p2, p3, COLOR_RED);
	}
}*/



