
#include "base_math.h"

///////////////////////////////////////
/// cjk:  Rect Functions 

RectU16 rect_u16(U16 x, U16 y, U16 width, U16 height){ return (RectU16){{x, y, width, height}}; };
RectU32 rect_u32(U32 x, U32 y, U32 width, U32 height){ return (RectU32){{x, y, width, height}}; };
RectU64 rect_u64(U64 x, U64 y, U64 width, U64 height){ return (RectU64){{x, y, width, height}}; };

RectF32 rect_f32(F32 x, F32 y, F32 width, F32 height){ return (RectF32){{x, y, width, height}}; };
RectF64 rect_f64(F64 x, F64 y, F64 width, F64 height){ return (RectF64){{x, y, width, height}}; };


///////////////////////////////////////
/// cjk:  Range Functions 

Rng1U32 rng_1u32(U32 min, U32 max) {
	Assert(min < max);
	return (Rng1U32){{.min = min, .max = max}};
}

Rng1F32 rng_1f32(F32 min, F32 max) {
	Assert(min < max);
	return (Rng1F32){{.min = min, .max = max}};
}

Rng1U64 rng_1u64(U64 min, U64 max) {
	Assert(min < max);
	return (Rng1U64){{.min = min, .max = max}};
}

Rng1F64 rng_1f64(F64 min, F64 max) {
	Assert(min < max);
	return (Rng1F64){{.min = min, .max = max}};
}

U64 dim_r1u64(Rng1U64 r) { 
	return ((r.max > r.min) ? (r.max - r.min) : 0); 
}

///////////////////////////////////////
/// cjk:  Point Functions 

Point2U16 point_2u16(U16 x, U16 y){ return (Point2U16){{x, y}};}
Point2U32 point_2u32(U32 x, U32 y){ return (Point2U32){{x, y}};}
Point2U64 point_2u64(U64 x, U64 y){ return (Point2U64){{x, y}};}

Point3U16 point_3u16(U16 x, U16 y, U16 z){ return (Point3U16){{x, y, z}};}
Point3U32 point_3U32(U32 x, U32 y, U32 z){ return (Point3U32){{x, y, z}};}
Point3U64 point_3U64(U64 x, U64 y, U64 z){ return (Point3U64){{x, y, z}};}

///////////////////////////////////////
/// cjk:  Vector Functions 

// Initializers
Vec2F32 vec_2f32(F32 x, F32 y){ return (Vec2F32){{x, y}};}
Vec2F64 vec_2f64(F64 x, F64 y){ return (Vec2F64){{x, y}};}

Vec3F32 vec_3f32(F32 x, F32 y, F32 z){ return (Vec3F32){{x, y, z}};}
Vec3F64 vec_3f64(F64 x, F64 y, F64 z){ return (Vec3F64){{x, y, z}};}

// dot products
F64 dot_vec2f32(Vec2F32 v1, Vec2F32 v2){ return (v1.x * v2.x) + (v2.y * v2.y);}
F64 dot_vec2f64(Vec2F64 v1, Vec2F64 v2){ return (v1.x * v2.x) + (v2.y * v2.y);}

F64 dot_vec3f32(Vec3F32 v1, Vec3F32 v2){ return (v1.x * v2.x) + (v2.y * v2.y) + (v1.z * v2.z);}
F64 dot_vec3f64(Vec3F64 v1, Vec3F64 v2){ return (v1.x * v2.x) + (v2.y * v2.y) + (v1.z * v2.z);}


