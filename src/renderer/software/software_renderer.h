#ifndef SOFTWARE_RENDERER_H
#define SOFTWARE_RENDERER_H

#include <math.h>

// This method uses Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm  
// https://groups.csail.mit.edu/graphics/classes/6.837/F02/lectures/6.837-7_Line.pdf
// https://zingl.github.io/Bresenham.pdf
void sr_draw_line(Vec2F32 p1, Vec2F32 p2, ColorRGBA color);
void sr_draw_triangle(Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color); 
void sr_draw_rect(RectF32 rect, ColorRGBA color);

B32 sr_is_point_in_triangle(Vec2F32 point, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3);
void sr_draw_filled_triangle(Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color);

// rendering api functions
static void soft_r_init(R_RenderContext* ctx);
static void soft_r_begin_frame(R_RenderContext* ctx, R_RenderTarget target);
static void soft_r_submit_commands(R_RenderCommand* cmd, U64 count);
static void soft_r_end_frame(R_RenderContext* ctx);
static void soft_r_close(R_RenderContext* ctx);


#endif // SOFTWARE_RENDERER_H 


