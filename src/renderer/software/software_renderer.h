#ifndef SOFTWARE_RENDERER_H
#define SOFTWARE_RENDERER_H

#include <math.h>

typedef U64 SR_ObjectRenderFlag;
enum{
	SR_ObjectRenderFlag_WireFrame,
	SR_ObjectRenderFlag_Texture,
	SR_ObjectRenderFlag_Filled,
};


// This method uses Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm  
// https://groups.csail.mit.edu/graphics/classes/6.837/F02/lectures/6.837-7_Line.pdf
// https://zingl.github.io/Bresenham.pdf
void sr_draw_line(Vec2F32 p1, Vec2F32 p2, ColorRGBA color);
void sr_draw_triangle(Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color); 
B32 sr_is_point_in_triangle(Vec2F32 point, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3);
void sr_draw_filled_triangle(Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color);
void sr_draw_rect(RectF32 rect, ColorRGBA color);

// This method uses Midpoint circle algorithm for quickly drawing a circle
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm 
void sr_draw_circle(Vec2F32 center, F32 radius, ColorRGBA color);

void sr_draw_obj(FMT_OBJ_Object* obj_file, SR_ObjectRenderFlag flag);

Vec2F32 sr_viewport_transform(Vec3F32 vec, U32 width, U32 height);

#endif // SOFTWARE_RENDERER_H 


