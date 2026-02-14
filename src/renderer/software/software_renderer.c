
// This method uses Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm  
// https://groups.csail.mit.edu/graphics/classes/6.837/F02/lectures/6.837-7_Line.pdf
// https://zingl.github.io/Bresenham.pdf
void wm_draw_line(OS_GFX_WindowContext* ctx, Vec2F32 p1, Vec2F32 p2, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->stride / sizeof(ColorRGBA);
	S32 window_width = ctx->size.width;
	S32 window_height = ctx->size.height;
	U32 color_const = color_rgba_to_bgra(color).c;

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

void wm_draw_triangle(OS_GFX_WindowContext* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color){
	wm_draw_line(ctx, v1, v2, color);
	wm_draw_line(ctx, v2, v3, color);
	wm_draw_line(ctx, v3, v1, color);
} 

B32 wm_is_point_in_triangle(Vec2F32 point, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3){
	B32 result = false;	
		 		
	return result;	
}

void wm_draw_filled_triangle(OS_GFX_WindowContext* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->stride / sizeof(ColorRGBA);
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


// 2d primitive drawing
void wm_draw_rect(OS_GFX_WindowContext* ctx, RectF32 rect, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);

	
	U32 x1 = Max(0, rect.x);
	U32 y1 = Max(0, rect.y);
	U32 x2 = Min(ctx->size.width, (rect.x + rect.width));
	U32 y2 = Min(ctx->size.height, (rect.y + rect.height));

	if( x1 >= x2 || y1 >= y2 ) return;

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->stride / sizeof(ColorRGBA);
	U32 width = x2 - x1;
	U32 color_const = color_rgba_to_bgra(color).c;

	// this points to the first pixel in the rect
	U32* row_ptr = pixels + (y1 * stride) + x1;

	for (U32 y = y1; y < y2; y++){
		for (U32 x = 0; x < width; x++){
			row_ptr[x] = color_const; 
		}
		row_ptr += stride;
	}
}


// This method uses Midpoint circle algorithm for quickly drawing a circle
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm 
void wm_draw_circle(OS_GFX_WindowContext* ctx, Vec2F32 center, F32 radius, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);
	Assert(radius > 0.0);

	S32 x0 = (S32) floorf(center.x);
	S32 y0 = (S32) floorf(center.y);
	S32 r  = (S32) radius;

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->stride / sizeof(ColorRGBA);
	S32 window_width = (S32)ctx->size.width;
	S32 window_height = (S32)ctx->size.height;
	U32 color_const = color_rgba_to_bgra(color).c;

	S32 x = r;
	S32 y = 0;
	S32 decision = 1 - r;

	#define LOCAL_MACRO_DRAWSPAN(py, x_start, x_end) do{\
			if((py) >= (0) && (py) <= (window_height)) {\
      				S32 left = Max(0, (x_start));\
				S32 right = Min((window_width-1),(x_end));\
				for(S32 px = left; px < right; px ++){\
					pixels[(py) * stride + px] = color_const;\
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

