
// This method uses Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm  
// https://groups.csail.mit.edu/graphics/classes/6.837/F02/lectures/6.837-7_Line.pdf
// https://zingl.github.io/Bresenham.pdf
void wm_draw_line(WM_Context* ctx, Vec2F32 p1, Vec2F32 p2, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->bytes_per_line / sizeof(ColorRGBA);
	S32 window_width = ctx->size.width;
	S32 window_height = ctx->size.height;
	U32 color_const = color_rgba_to_bgra(color).c;

	S32 x0 = (S32) floorf(p1.x);
	S32 y0 = (S32) floorf(p1.y); 
	S32 x1 = (S32) floorf(p2.x); 
	S32 y1 = (S32) floorf(p2.y);


	S32 dx = abs(x1 - x0);
	S32 dy = -abs(y1 - y0);
	S32 sx = (x0 < x1)? 1 : -1;
	S32 sy = (y0 < y1)? 1 : -1;
	S32 err = dx+dy;
	S32 err_2;

	for(;;){
		if( 0 <= x0 && x0 < window_width && 0 <= y0 && y0 < window_height){
			pixels[y0 * stride + x0] = color_const;
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

void wm_draw_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color){
	wm_draw_line(ctx, v1, v2, color);
	wm_draw_line(ctx, v2, v3, color);
	wm_draw_line(ctx, v3, v1, color);
} 

B32 wm_is_point_in_triangle(Vec2F32 point, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3){
	B32 result = false;	
		 		
	return result;	
}

void wm_draw_filled_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->bytes_per_line / sizeof(ColorRGBA);
	U32 window_width = ctx->size.width;
	U32 window_height = ctx->size.height;
	U32 color_const = color_rgba_to_bgra(color).c;

	// get bounding box
	U32 x_min = Min(0, Min(v1.x, Min(v2.x, v3.x)));
	U32 y_min = Min(0, Min(v1.y, Min(v2.y, v3.y)));
	U32 x_max = Min(window_width, Max(v1.x, Max(v2.x, v3.x)));
	U32 y_max = Min(window_height, Max(v1.y, Max(v2.y, v3.y)));

	Rng1U32 x_rng = Rng1_U32(x_min, x_max);
	Rng1U32 y_rng = Rng1_U32(y_min, y_max);

	for EachInRange(y, y_rng){
		for EachInRange(x, x_rng){
			if(wm_is_point_in_triangle(Vec2_F32(x, y), v1, v2, v3)){
				pixels[y * stride + x] = color_const;
			}
		}	
	}
}


void wm_close_window(WM_Context* ctx){
	XUnmapWindow(ctx->display, ctx->window);
	XDestroyWindow(ctx->display, ctx->window);
	XFreeGC(ctx->display, ctx->graphics_ctx);
	XCloseDisplay(ctx->display);
}

void wm_register_input_events(WM_Context* ctx, WM_EventFlag flags){
	long event_mask = 0;	

	for EachIndex(i, ArrayCount(wm_event_mask_map)){
		if(flags & wm_event_mask_map[i].flag){
			event_mask |= wm_event_mask_map[i].event_mask;
		}
	}
	
	XSelectInput(ctx->display, ctx->window, event_mask);
}

U32 wm_num_of_pending_events(WM_Context* ctx){
	return XPending(ctx->display);
}
