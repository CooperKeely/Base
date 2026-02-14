#ifndef SOFTWARE_RENDERER_H
#define SOFTWARE_RENDERER_H

#include <math.h>

// This method uses Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm  
// https://groups.csail.mit.edu/graphics/classes/6.837/F02/lectures/6.837-7_Line.pdf
// https://zingl.github.io/Bresenham.pdf
void wm_draw_line(OS_GFX_WindowContext* ctx, Vec2F32 p1, Vec2F32 p2, ColorRGBA color);
void wm_draw_triangle(OS_GFX_WindowContext* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color); 
B32 wm_is_point_in_triangle(Vec2F32 point, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3);
void wm_draw_filled_triangle(OS_GFX_WindowContext* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color);
void wm_draw_rect(OS_GFX_WindowContext* ctx, RectF32 rect, ColorRGBA color);

// This method uses Midpoint circle algorithm for quickly drawing a circle
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm 
void wm_draw_circle(OS_GFX_WindowContext* ctx, Vec2F32 center, F32 radius, ColorRGBA color);


#endif // SOFTWARE_RENDERER_H 


