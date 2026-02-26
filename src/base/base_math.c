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
	Assert(min <= max);
	return (Rng1U32){{.min = min, .max = max}};
}

Rng1F32 rng_1f32(F32 min, F32 max) {
	Assert(min <= max);
	return (Rng1F32){{.min = min, .max = max}};
}

Rng1U64 rng_1u64(U64 min, U64 max) {
	Assert(min <= max);
	return (Rng1U64){{.min = min, .max = max}};
}

Rng1F64 rng_1f64(F64 min, F64 max) {
	Assert(min <= max);
	return (Rng1F64){{.min = min, .max = max}};
}

U64 dim_r1u64(Rng1U64 r) { 
	return ((r.max > r.min) ? (r.max - r.min) : 0); 
}

///////////////////////////////////////
/// cjk:  Point Functions 

Pnt2U16 pnt_2u16(U16 x, U16 y){ return (Pnt2U16){{x, y}};}
Pnt2U32 pnt_2u32(U32 x, U32 y){ return (Pnt2U32){{x, y}};}
Pnt2U64 pnt_2u64(U64 x, U64 y){ return (Pnt2U64){{x, y}};}

Pnt3U16 pnt_3u16(U16 x, U16 y, U16 z){ return (Pnt3U16){{x, y, z}};}
Pnt3U32 pnt_3U32(U32 x, U32 y, U32 z){ return (Pnt3U32){{x, y, z}};}
Pnt3U64 pnt_3U64(U64 x, U64 y, U64 z){ return (Pnt3U64){{x, y, z}};}

///////////////////////////////////////
/// cjk:  Vector Functions 

Vec2F32 vec_2f32(F32 x, F32 y)			{ return (Vec2F32){{x, y}}; }
F32 	dot_vec2f32(Vec2F32 v1, Vec2F32 v2)	{ return (v1.x * v2.x) + (v1.y * v2.y);}
Vec2F32 add_vec2f32(Vec2F32 v1, Vec2F32 v2)	{ return (Vec2F32){{v1.x + v2.x, v1.y + v2.y}};}
Vec2F32 sub_vec2f32(Vec2F32 v1, Vec2F32 v2)	{ return (Vec2F32){{v1.x - v2.x, v1.y - v2.y}};}
Vec2F32 mul_vec2f32(Vec2F32 v1, Vec2F32 v2)	{ return (Vec2F32){{v1.x * v2.x, v1.y * v2.y}};}
Vec2F32 div_vec2f32(Vec2F32 v1, Vec2F32 v2)	{ return (Vec2F32){{v1.x / v2.x, v1.y / v2.y}};}
Vec2F32 scale_vec2f32(Vec2F32 v1, F32 s)	{ return (Vec2F32){{v1.x * s, v1.y * s}}; }
F32	length_squared_vec2f32(Vec2F32 v1)	{ return (v1.x * v1.x) + (v1.y * v1.y); }
F32 	length_vec2f32(Vec2F32 v1)		{ return sqrt_f32(length_squared_vec2f32(v1)); }
Vec2F32	normalize_vec2f32(Vec2F32 v1)		{ return scale_vec2f32(v1, F32Lit(1.0)/length_vec2f32(v1));}

Vec2F64 vec_2f64(F64 x, F64 y)			{ return (Vec2F64){{x, y}}; }
F64 	dot_vec2f64(Vec2F64 v1, Vec2F64 v2)	{ return (v1.x * v2.x) + (v1.y * v2.y);}
Vec2F64 add_vec2f64(Vec2F64 v1, Vec2F64 v2)	{ return (Vec2F64){{v1.x + v2.x, v1.y + v2.y}};}
Vec2F64 sub_vec2f64(Vec2F64 v1, Vec2F64 v2)	{ return (Vec2F64){{v1.x - v2.x, v1.y - v2.y}};}
Vec2F64 mul_vec2f64(Vec2F64 v1, Vec2F64 v2)	{ return (Vec2F64){{v1.x * v2.x, v1.y * v2.y}};}
Vec2F64 div_vec2f64(Vec2F64 v1, Vec2F64 v2)	{ return (Vec2F64){{v1.x / v2.x, v1.y / v2.y}};}
Vec2F64 scale_vec2f64(Vec2F64 v1, F64 s)	{ return (Vec2F64){{v1.x * s, v1.y * s}}; }
F64	length_squared_vec2f64(Vec2F64 v1)	{ return (v1.x * v1.x) + (v1.y * v1.y); }
F64 	length_vec2f64(Vec2F64 v1)		{ return sqrt_f64(length_squared_vec2f64(v1)); }
Vec2F64	normalize_vec2f64(Vec2F64 v1)		{ return scale_vec2f64(v1, F64Lit(1.0)/length_vec2f64(v1));}

Vec3F32 vec_3f32(F32 x, F32 y, F32 z)		{ return (Vec3F32){{x, y, z}}; }
F32 	dot_vec3f32(Vec3F32 v1, Vec3F32 v2)	{ return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);}
Vec3F32 add_vec3f32(Vec3F32 v1, Vec3F32 v2)	{ return (Vec3F32){{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}};}
Vec3F32 sub_vec3f32(Vec3F32 v1, Vec3F32 v2)	{ return (Vec3F32){{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}};}
Vec3F32 mul_vec3f32(Vec3F32 v1, Vec3F32 v2)	{ return (Vec3F32){{v1.x * v2.x, v1.y * v2.y, v1.z * v2.z}};}
Vec3F32 div_vec3f32(Vec3F32 v1, Vec3F32 v2)	{ return (Vec3F32){{v1.x / v2.x, v1.y / v2.y, v1.z / v2.z}};}
Vec3F32 scale_vec3f32(Vec3F32 v1, F32 s)	{ return (Vec3F32){{v1.x * s, v1.y * s, v1.z * s}}; }
F32	length_squared_vec3f32(Vec3F32 v1)	{ return (v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z); }
F32 	length_vec3f32(Vec3F32 v1)		{ return sqrt_f32(length_squared_vec3f32(v1)); }
Vec3F32	normalize_vec3f32(Vec3F32 v1)		{ return scale_vec3f32(v1, F32Lit(1.0)/length_vec3f32(v1));}

Vec3F64 vec_3f64(F64 x, F64 y, F64 z)		{ return (Vec3F64){{x, y, z}}; }
F64 	dot_vec3f64(Vec3F64 v1, Vec3F64 v2)	{ return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);}
Vec3F64 add_vec3f64(Vec3F64 v1, Vec3F64 v2)	{ return (Vec3F64){{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}};}
Vec3F64 sub_vec3f64(Vec3F64 v1, Vec3F64 v2)	{ return (Vec3F64){{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}};}
Vec3F64 mul_vec3f64(Vec3F64 v1, Vec3F64 v2)	{ return (Vec3F64){{v1.x * v2.x, v1.y * v2.y, v1.z * v2.z}};}
Vec3F64 div_vec3f64(Vec3F64 v1, Vec3F64 v2)	{ return (Vec3F64){{v1.x / v2.x, v1.y / v2.y, v1.z / v2.z}};}
Vec3F64 scale_vec3f64(Vec3F64 v1, F64 s)	{ return (Vec3F64){{v1.x * s, v1.y * s, v1.z * s}}; }
F64	length_squared_vec3f64(Vec3F64 v1)	{ return (v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z); }
F64 	length_vec3f64(Vec3F64 v1)		{ return sqrt_f64(length_squared_vec3f64(v1)); }
Vec3F64	normalize_vec3f64(Vec3F64 v1)		{ return scale_vec3f64(v1, F64Lit(1.0)/length_vec3f64(v1));}

Vec4F32 vec_4f32(F32 x, F32 y, F32 z, F32 w)	{ return (Vec4F32){{x, y, z, w}}; }
F32 	dot_vec4f32(Vec4F32 v1, Vec4F32 v2)	{ return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);}
Vec4F32 add_vec4f32(Vec4F32 v1, Vec4F32 v2)	{ return (Vec4F32){{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w}};}
Vec4F32 sub_vec4f32(Vec4F32 v1, Vec4F32 v2)	{ return (Vec4F32){{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w}};}
Vec4F32 mul_vec4f32(Vec4F32 v1, Vec4F32 v2)	{ return (Vec4F32){{v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w}};}
Vec4F32 div_vec4f32(Vec4F32 v1, Vec4F32 v2)	{ return (Vec4F32){{v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w}};}
Vec4F32 scale_vec4f32(Vec4F32 v1, F32 s)	{ return (Vec4F32){{v1.x * s, v1.y * s, v1.z * s, v1.w * s}}; }
F32	length_squared_vec4f32(Vec4F32 v1)	{ return (v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z) + (v1.w * v1.w); }
F32 	length_vec4f32(Vec4F32 v1)		{ return sqrt_f32(length_squared_vec4f32(v1)); }
Vec4F32	normalize_vec4f32(Vec4F32 v1)		{ return scale_vec4f32(v1, F32Lit(1.0)/length_vec4f32(v1));}

Vec4F64 vec_4f64(F64 x, F64 y, F64 z, F64 w)	{ return (Vec4F64){{x, y, z, w}}; }
F64 	dot_vec4f64(Vec4F64 v1, Vec4F64 v2)	{ return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);}
Vec4F64 add_vec4f64(Vec4F64 v1, Vec4F64 v2)	{ return (Vec4F64){{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w}};}
Vec4F64 sub_vec4f64(Vec4F64 v1, Vec4F64 v2)	{ return (Vec4F64){{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w}};}
Vec4F64 mul_vec4f64(Vec4F64 v1, Vec4F64 v2)	{ return (Vec4F64){{v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w}};}
Vec4F64 div_vec4f64(Vec4F64 v1, Vec4F64 v2)	{ return (Vec4F64){{v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w}};}
Vec4F64 scale_vec4f64(Vec4F64 v1, F64 s)	{ return (Vec4F64){{v1.x * s, v1.y * s, v1.z * s, v1.w * s}}; }
F64	length_squared_vec4f64(Vec4F64 v1)	{ return (v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z) + (v1.w * v1.w); }
F64 	length_vec4f64(Vec4F64 v1)		{ return sqrt_f64(length_squared_vec4f64(v1)); }
Vec4F64	normalize_vec4f64(Vec4F64 v1)		{ return scale_vec4f64(v1, F64Lit(1.0)/length_vec4f64(v1));}


///////////////////////////////////////
/// cjk:  Vector Functions 

Mat3x3F32 mat_3x3f32(F32 diagonal){
	return (Mat3x3F32){
		.m = {
			{diagonal, 0.0f, 0.0f},
			{0.0f, diagonal, 0.0f},
			{0.0f, 0.0f, diagonal}
		}
	};
}

Mat3x3F32 make_translate_3x3f32(Vec2F32 delta){
	Mat3x3F32 identity = mat_3x3f32(1.0f);
	identity.a13 = delta.x;
	identity.a23 = delta.y;
	return identity;
}

Mat3x3F32 make_scale_3x3f32(Vec2F32 scale){
	Mat3x3F32 identity = mat_3x3f32(1.0f);
	identity.a11 *= scale.x;
	identity.a22 *= scale.y;
	return identity;
}

Mat3x3F32 mul_3x3f32(Mat3x3F32 a, Mat3x3F32 b){
	Mat3x3F32 mat = mat_3x3f32(0.0f);
	for EachIndex(i, 3){
		for EachIndex(j, 3){
			for EachIndex(k, 3){
				mat.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return mat;
}


Mat4x4F32 mat_4x4f32(F32 diagonal){
	return (Mat4x4F32){
		.m = {
			{diagonal, 0.0f, 0.0f, 0.0f},
			{0.0f, diagonal, 0.0f, 0.0f},
			{0.0f, 0.0f, diagonal, 0.0f},
			{0.0f, 0.0f, 0.0f, diagonal}
		}
	};
} 

Mat4x4F32 make_translate_4x4f32(Vec3F32 delta){
	Mat4x4F32 identity = mat_4x4f32(1.0f);
	identity.a14 = delta.x;
	identity.a24 = delta.y;
	identity.a34 = delta.z;
	return identity;
}

Mat4x4F32 make_scale_4x4f32(Vec3F32 scale){
	Mat4x4F32 identity = mat_4x4f32(1.0f);
	identity.a11 *= scale.x;
	identity.a22 *= scale.y;
	identity.a33 *= scale.z;
	return identity;

}

Mat4x4F32 make_perspective_4x4f32(F32 fov, F32 aspect_ratio, F32 near_z, F32 far_z);
Mat4x4F32 make_orthographic_4x4f32(F32 left, F32 right, F32 bottom, F32 top, F32 near_z, F32 far_z);
Mat4x4F32 make_look_at_4x4f32(Vec3F32 eye, Vec3F32 center, Vec3F32 up);
Mat4x4F32 make_rotate_4x4f32(Vec3F32 axis, F32 turns);

Mat4x4F32 mul_4x4f32(Mat4x4F32 a, Mat4x4F32 b){
	Mat4x4F32 mat = mat_4x4f32(0.0f);
	for EachIndex(i, 4){
		for EachIndex(j, 4){
			for EachIndex(k, 4){
				mat.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return mat;
}

Mat4x4F32 scale_4x4f32(Mat4x4F32 mat, F32 scale){
	Mat4x4F32 scale_mat = make_scale_4x4f32(scale);
	return mul_4x4f32(scale_mat, mat);
}
Mat4x4F32 inverse_4x4f32(Mat4x4F32 m);
Mat4x4F32 derotate_4x4f32(Mat4x4F32 mat);
Mat4x4F32 transpose_4x4f32(Mat4x4F32 mat);


