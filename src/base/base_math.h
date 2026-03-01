#ifndef BASE_MATH_H
#define BASE_MATH_H

#include <math.h>

///////////////////////////////////////
/// cjk: Math Helper Functions 

#define sqrt_f32(x)			sqrtf(x)
#define cbrt_f32(x)			cbrtf(x)
#define mod_f32(a, b)			fmodf((a), (b))
#define pow_f32(a, b)			powf((a), (b))
#define ceil_f32(x)			ceilf(x)
#define floor_f32(x)			floorf(x)
#define round_f32(x)			roundf(x)
#define abs_f32(x)			fabsf(x)
#define radians_from_turns_f32(x)	((x) * (2 * pi32))
#define turns_from_radians_f32(x)	((x) / (2 * pi32))
#define degrees_from_turns_f32(x)	((x) * F32Lit(360.0))
#define turns_from_degrees_f32(x)	((x) / F32Lit(360.0))
#define degrees_from_radians_f32(x)	(degrees_from_turns_f32(turns_from_radians_f32((x)))
#define radians_from_degrees_f32(x)	(radians_from_turns_f32(turns_from_degrees_f32((x)))
#define sin_f32(x)			sinf(radians_from_turns_f32(x))
#define cos_f32(x)			cosf(radians_from_turns_f32(x))
#define tan_f32(x)			tanf(radians_from_turns_f32(x))

#define sqrt_f64(x)			sqrt(x)
#define cbrt_f64(x)			cbrt(x)
#define mod_f64(a, b)			fmod((a), (b))
#define pow_f64(a, b)			pow((a), (b))
#define ceil_f64(x)			ceil(x)
#define floor_f64(x)			floor(x)
#define round_f64(x)			round(x)
#define abs_f64(x)			fabs(x)
#define radians_from_turns_f64(x)	((x) * (2 * pi32))
#define turns_from_radians_f64(x)	((x) / (2 * pi32))
#define degrees_from_turns_f64(x)	((x) * F64Lit(360.0))
#define turns_from_degrees_f64(x)	((x) / F64Lit(360.0))
#define degrees_from_radians_f64(x)	(degrees_from_turns_f64(turns_from_radians_f64((x)))
#define radians_from_degrees_f64(x)	(radians_from_turns_f64(turns_from_degrees_f64((x)))
#define sin_f64(x)			sin(radians_from_turns_f64(x))
#define cos_f64(x)			cos(radians_from_turns_f64(x))
#define tan_f64(x)			tan(radians_from_turns_f64(x))


///////////////////////////////////////
/// cjk: Point Type Definitions 

typedef union {
	struct {
		U16 x;
		U16 y;
	};
	U16 v[2];
} Pnt2U16;

typedef union {
	struct {
		U32 x;
		U32 y;
	};
	U32 v[2];
} Pnt2U32;

typedef union {
	struct {
		U64 x;
		U64 y;
	};
	U64 v[2];
} Pnt2U64;

typedef union {
	struct {
		U16 x;
		U16 y;
		U16 z;
	};
	U16 v[3];
} Pnt3U16;

typedef union {
	struct {
		U32 x;
		U32 y;
		U32 z;
	};
	U32 v[3];
} Pnt3U32;

typedef union {
	struct {
		U64 x;
		U64 y;
		U64 z;
	};
	U64 v[3];
} Pnt3U64;

#define Pnt2_U16(x, y)  	pnt_2u16((x), (y))
#define Pnt2_U32(x, y) 		pnt_2u32((x), (y)) 
#define Pnt2_U64(x, y) 		pnt_2u64((x), (y)) 

#define Pnt3_U16(x, y, z)  	pnt_3u16((x), (y), (z))
#define Pnt3_U32(x, y, z)  	pnt_3u32((x), (y), (z))
#define Pnt3_U64(x, y, z)  	pnt_3u64((x), (y), (z))

Pnt2U16 pnt_2u16(U16 x, U16 y);
Pnt2U32 pnt_2u32(U32 x, U32 y);
Pnt2U64 pnt_2u64(U64 x, U64 y);

Pnt3U16 pnt_3u16(U16 x, U16 y, U16 z);
Pnt3U32 pnt_3u32(U32 x, U32 y, U32 z);
Pnt3U64 pnt_3u64(U64 x, U64 y, U64 z);



///////////////////////////////////////
/// cjk: Vector Types and Functions 

typedef union {
	struct {
		F32 x;
		F32 y;
	};
	F32 v[2];
} Vec2F32;

typedef union {
	struct {
		F64 x;
		F64 y;
	};
	F64 v[2];
} Vec2F64;

typedef union {
	struct {
		F32 x;
		F32 y;
		F32 z;
	};
	F32 v[3];
} Vec3F32;


typedef union {
	struct {
		F64 x;
		F64 y;
		F64 z;
	};
	F64 v[3];
} Vec3F64;

typedef union {
	struct {
		F32 x;
		F32 y;
		F32 z;
		F32 w;
	};
	F32 v[4];
} Vec4F32;


typedef union {
	struct {
		F64 x;
		F64 y;
		F64 z;
		F64 w;
	};
	F64 v[4];
} Vec4F64;


#define Vec2_F32(x, y)		vec_2f32((x), (y))
#define Vec2_F64(x, y)		vec_2f64((x), (y))

#define Vec3_F32(x, y, z)	vec_3f32((x), (y), (z))
#define Vec3_F64(x, y, z)	vec_3f64((x), (y), (z))

#define Vec4_F32(x, y, z)	vec_4f32((x), (y), (z))
#define Vec4_F64(x, y, z)	vec_4f64((x), (y), (z))

// Vector Functions 

Vec2F32 vec_2f32(F32 x, F32 y);
F32 	dot_vec2f32(Vec2F32 v1, Vec2F32 v2);
Vec2F32 add_vec2f32(Vec2F32 v1, Vec2F32 v2);
Vec2F32 sub_vec2f32(Vec2F32 v1, Vec2F32 v2);
Vec2F32 mul_vec2f32(Vec2F32 v1, Vec2F32 v2);
Vec2F32 div_vec2f32(Vec2F32 v1, Vec2F32 v2);
Vec2F32 scale_vec2f32(Vec2F32 v1, F32 s);
F32	length_squared_vec2f32(Vec2F32 v1);
F32 	length_vec2f32(Vec2F32 v1);
Vec2F32	normalize_vec2f32(Vec2F32 v1);

Vec2F64 vec_2f64(F64 x, F64 y);
F64 	dot_vec2f64(Vec2F64 v1, Vec2F64 v2);
Vec2F64 add_vec2f64(Vec2F64 v1, Vec2F64 v2);
Vec2F64 sub_vec2f64(Vec2F64 v1, Vec2F64 v2);
Vec2F64 mul_vec2f64(Vec2F64 v1, Vec2F64 v2);
Vec2F64 div_vec2f64(Vec2F64 v1, Vec2F64 v2);
Vec2F64 scale_vec2f64(Vec2F64 v1, F64 s);
F64	length_squared_vec2f64(Vec2F64 v1);
F64 	length_vec2f64(Vec2F64 v1);
Vec2F64	normalize_vec2f64(Vec2F64 v1);

Vec3F32 vec_3f32(F32 x, F32 y, F32 z);
F32 	dot_vec3f32(Vec3F32 v1, Vec3F32 v2);
Vec3F32 add_vec3f32(Vec3F32 v1, Vec3F32 v2);
Vec3F32 sub_vec3f32(Vec3F32 v1, Vec3F32 v2);
Vec3F32 mul_vec3f32(Vec3F32 v1, Vec3F32 v2);
Vec3F32 div_vec3f32(Vec3F32 v1, Vec3F32 v2);
Vec3F32 scale_vec3f32(Vec3F32 v1, F32 s);
F32	length_squared_vec3f32(Vec3F32 v1);
F32 	length_vec3f32(Vec3F32 v1);
Vec3F32	normalize_vec3f32(Vec3F32 v1);

Vec3F64 vec_3f64(F64 x, F64 y, F64 z);
F64 	dot_vec3f64(Vec3F64 v1, Vec3F64 v2);
Vec3F64 add_vec3f64(Vec3F64 v1, Vec3F64 v2);
Vec3F64 sub_vec3f64(Vec3F64 v1, Vec3F64 v2);
Vec3F64 mul_vec3f64(Vec3F64 v1, Vec3F64 v2);
Vec3F64 div_vec3f64(Vec3F64 v1, Vec3F64 v2);
Vec3F64 scale_vec3f64(Vec3F64 v1, F64 s);
F64	length_squared_vec3f64(Vec3F64 v1);
F64 	length_vec3f64(Vec3F64 v1);
Vec3F64	normalize_vec3f64(Vec3F64 v1);

Vec4F32 vec_4f32(F32 x, F32 y, F32 z, F32 w);
F32 	dot_vec4f32(Vec4F32 v1, Vec4F32 v2);
Vec4F32 add_vec4f32(Vec4F32 v1, Vec4F32 v2);
Vec4F32 sub_vec4f32(Vec4F32 v1, Vec4F32 v2);
Vec4F32 mul_vec4f32(Vec4F32 v1, Vec4F32 v2);
Vec4F32 div_vec4f32(Vec4F32 v1, Vec4F32 v2);
Vec4F32 scale_vec4f32(Vec4F32 v1, F32 s);
F32	length_squared_vec4f32(Vec4F32 v1);
F32 	length_vec4f32(Vec4F32 v1);
Vec4F32	normalize_vec4f32(Vec4F32 v1);

Vec4F64 vec_4f64(F64 x, F64 y, F64 z, F64 w);
F64 	dot_vec4f64(Vec4F64 v1, Vec4F64 v2);
Vec4F64 add_vec4f64(Vec4F64 v1, Vec4F64 v2);
Vec4F64 sub_vec4f64(Vec4F64 v1, Vec4F64 v2);
Vec4F64 mul_vec4f64(Vec4F64 v1, Vec4F64 v2);
Vec4F64 div_vec4f64(Vec4F64 v1, Vec4F64 v2);
Vec4F64 scale_vec4f64(Vec4F64 v1, F64 s);
F64	length_squared_vec4f64(Vec4F64 v1);
F64 	length_vec4f64(Vec4F64 v1);
Vec4F64	normalize_vec4f64(Vec4F64 v1);

///////////////////////////////////////
/// cjk: Matrix Definitions

typedef union {
	struct {
		F32 a11, a12, a13;
		F32 a21, a22, a23;
		F32 a31, a32, a33;
	};
	F32 m[3][3];
} Mat3x3F32;

typedef union {
	struct {
		F32 a11, a12, a13, a14;
		F32 a21, a22, a23, a24;
		F32 a31, a32, a33, a34;
		F32 a41, a42, a43, a44;
	};
	F32 m[4][4];
} Mat4x4F32;

// matrix functions
Mat3x3F32 mat_3x3f32(F32 diagonal);
Mat3x3F32 make_translate_3x3f32(Vec2F32 delta);
Mat3x3F32 make_scale_3x3f32(Vec2F32 scale);
Mat3x3F32 mul_3x3f32(Mat3x3F32 a, Mat3x3F32 b);

Mat4x4F32 mat_4x4f32(F32 diagonal);
Mat4x4F32 make_translate_4x4f32(Vec3F32 delta);
Mat4x4F32 make_scale_4x4f32(Vec3F32 scale);
Mat4x4F32 make_perspective_4x4f32(F32 fov, F32 aspect_ratio, F32 near_z, F32 far_z);
Mat4x4F32 make_orthographic_4x4f32(F32 left, F32 right, F32 bottom, F32 top, F32 near_z, F32 far_z);
Mat4x4F32 make_look_at_4x4f32(Vec3F32 eye, Vec3F32 center, Vec3F32 up);
Mat4x4F32 make_rotate_4x4f32(Vec3F32 axis, F32 turns);
Mat4x4F32 mul_4x4f32(Mat4x4F32 a, Mat4x4F32 b);
Mat4x4F32 scale_4x4f32(Mat4x4F32 m, F32 scale);
Mat4x4F32 inverse_4x4f32(Mat4x4F32 m);
Mat4x4F32 derotate_4x4f32(Mat4x4F32 mat);
Mat4x4F32 transpose_4x4f32(Mat4x4F32 mat);



///////////////////////////////////////
/// cjk: Range Definitions 

typedef union {
	struct {
		U32 min;
		U32 max;
	};
	U32 v[2];
} Rng1U32;

typedef union {
	struct {
		F32 min;
		F32 max;
	};
	F32 v[2];
} Rng1F32;

typedef union {
	struct {
		U64 min;
		U64 max;
	};
	U64 v[2];
} Rng1U64;

typedef union {
	struct {
		F64 min;
		F64 max;
	};
	F64 v[2];
} Rng1F64;

#define Rng1_U32(min, max) 	rng_1u32((min), (max))
#define Rng1_F32(min, max) 	rng_1f32((min), (max))
#define Rng1_U64(min, max) 	rng_1u64((min), (max))
#define Rng1_F64(min, max) 	rng_1f64((min), (max))

Rng1U32 rng_1u32(U32 min, U32 max);
Rng1F32 rng_1f32(F32 min, F32 max);
Rng1U64 rng_1u64(U64 min, U64 max);
Rng1F64 rng_1f64(F64 min, F64 max);

U64 dim_r1u64(Rng1U64 range);


///////////////////////////////////////
/// cjk: Rect Definitions 

typedef union{
	struct{
		U16 x;
		U16 y;
		U16 width;
		U16 height;
	};
	U16 v[4];
} RectU16;

typedef union{
	struct{
		U32 x;
		U32 y;
		U32 width;
		U32 height;
	};
	U32 v[4];
} RectU32;

typedef union{
	struct{
		U64 x;
		U64 y;
		U64 width;
		U64 height;
	};
	U64 v[4];
} RectU64;

typedef union{
	struct{
		F32 x;
		F32 y;
		F32 width;
		F32 height;
	};
	F32 v[4];
} RectF32;

typedef union{
	struct{
		F64 x;
		F64 y;
		F64 width;
		F64 height;
	};
	F64 v[4];
} RectF64;



#define Rect_U16(x, y, w, h) 	rect_u16((x), (y), (w), (h))
#define Rect_U32(x, y, w, h) 	rect_u32((x), (y), (w), (h))
#define Rect_U64(x, y, w, h) 	rect_u64((x), (y), (w), (h))

#define Rect_F32(x, y, w, h) 	rect_f32((x), (y), (w), (h))
#define Rect_F64(x, y, w, h) 	rect_f64((x), (y), (w), (h))

RectU16 rect_u16(U16 x, U16 y, U16 width, U16 height);
RectU32 rect_u32(U32 x, U32 y, U32 width, U32 height);
RectU64 rect_u64(U64 x, U64 y, U64 width, U64 height);

RectF32 rect_f32(F32 x, F32 y, F32 width, F32 height);
RectF64 rect_f64(F64 x, F64 y, F64 width, F64 height);


///////////////////////////////////////
/// cjk: PRNG Definitions 
/// https://en.wikipedia.org/wiki/Mersenne_Twister

// TODO: (cjk) implement Mersenne Twiser psuedo random number generation


#endif //BASE_MATH_H
