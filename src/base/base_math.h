#ifndef BASE_MATH_H
#define BASE_MATH_H

///////////////////////////////////////
/// cjk: Point Type Definitions 

typedef union {
	struct {
		U16 x;
		U16 y;
	};
	U16 v[2];
} Point2U16;

typedef union {
	struct {
		U32 x;
		U32 y;
	};
	U32 v[2];
} Point2U32;

typedef union {
	struct {
		U64 x;
		U64 y;
	};
	U64 v[2];
} Point2U64;

typedef union {
	struct {
		U16 x;
		U16 y;
		U16 z;
	};
	U16 v[3];
} Point3U16;

typedef union {
	struct {
		U32 x;
		U32 y;
		U32 z;
	};
	U32 v[3];
} Point3U32;

typedef union {
	struct {
		U64 x;
		U64 y;
		U64 z;
	};
	U64 v[3];
} Point3U64;

#define p2_U16(x, y)  	point_2u16((x), (y))
#define p2_U32(x, y) 	point_2u32((x), (y)) 
#define p2_U64(x, y) 	point_2u64((x), (y)) 

#define Point3_U16(x, y, z)  	point_3u16((x), (y), (z))
#define Point3_U32(x, y, z)  	point_3u32((x), (y), (z))
#define Point3_U64(x, y, z)  	point_3u64((x), (y), (z))

Point2U16 point_2u16(U16 x, U16 y);
Point2U32 point_2u32(U32 x, U32 y);
Point2U64 point_2u64(U64 x, U64 y);

Point3U16 point_3u16(U16 x, U16 y, U16 z);
Point3U32 point_3u32(U32 x, U32 y, U32 z);
Point3U64 point_3u64(U64 x, U64 y, U64 z);



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



Vec2F32 vec_2f32(F32 x, F32 y);
Vec2F64 vec_2f64(F64 x, F64 y);

Vec3F32 vec_3f32(F32 x, F32 y, F32 z);
Vec3F64 vec_3f64(F64 x, F64 y, F64 z);

Vec4F32 vec_4f32(F32 x, F32 y, F32 z, F32 w);
Vec4F64 vec_4f64(F64 x, F64 y, F64 z, F64 w);

F64 dot_vec2f32(Vec2F32 v1, Vec2F32 v2);
F64 dot_vec2f64(Vec2F64 v1, Vec2F64 v2);

F64 dot_vec3f32(Vec3F32 v1, Vec3F32 v2);
F64 dot_vec3f64(Vec3F64 v1, Vec3F64 v2);

F64 dot_vec4f32(Vec4F32 v1, Vec4F32 v2);
F64 dot_vec4f64(Vec4F64 v1, Vec4F64 v2);


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
