///////////////////////////////////////
/// cjk: Memory Helper Functions 

B32 memory_is_zero(void* ptr, U64 size){
	B32 result = BASE_TRUE;

	U64 extra = (size&0x07);
	U64 count8 = (size >> 3);
	
	// check data with 8 byte stride
	U64 *p64 = (U64*) ptr;
	if(result){
		for(U64 i = 0; i < count8; i += 1, p64 += 1){
			if(*p64 != 0){
				result = BASE_FALSE;
				goto done;
			}
		}

	}
	
	if(result){
		U8* p8 = (U8*) p64;
		for(U64 i = 0; i < extra; i += 1, p8 += 1){
			if(*p8 != 0){
				result = BASE_FALSE;
				goto done;
			}
		}

	}

done:;
	return result;

}

///////////////////////////////////////
/// cjk: Base Integer Functions 

U8 safe_cast_U8(U16 x) {
	Assert(x < max_U8);
	return (U8)x;
}

U16 safe_cast_U16(U32 x) {
	Assert(x < max_U16);
	return (U16)x;
}

U32 safe_cast_U32(U64 x) {
	Assert(x < max_U32);
	return (U32)x;
}

S8 safe_cast_S8(S16 x) {
	Assert(x < max_S8);
	Assert(x > min_S8);
	return (S8)x;
}

S16 safe_cast_S16(S32 x) {
	Assert(x < max_S16);
	Assert(x > min_S16);
	return (S16)x;
}

S32 safe_cast_S32(S64 x) {
	Assert(x < max_S32);
	Assert(x > min_S32);
	return (S32)x;
}


///////////////////////////////////////
/// cjk: Time Functions

DenseTime densetime_from_datetime(DateTime time) {
	DenseTime result = 0;
	result += time.year;
	result *= 12;
	result += time.month;
	result *= 31;
	result += time.day;
	result *= 24;
	result += time.hour;
	result *= 60;
	result += time.min;
	result *= 61;
	result += time.sec;
	result *= 1000;
	result += time.mil_sec;
	return result;
}

DateTime datetime_from_densetime(DenseTime time) {
	DateTime result = {0};
	result.mil_sec = time % 1000;
	time /= 1000;
	result.sec = time % 61;
	time /= 61;
	result.min = time % 60;
	time /= 60;
	result.hour = time % 24;
	time /= 24;
	result.day = time % 31;
	time /= 31;
	result.month_num = time % 12;
	time /= 12;
	Assert(time < max_U32);
	result.year = (U32)time;
	return result;
}
///////////////////////////////////////
/// cjk: Color Functions 

ColorRGBA color_rgba(U8 r, U8 g, U8 b, U8 a){ return (ColorRGBA){{r, g, b,a}};}

// TODO: inline color conversion
inline ColorBGRA color_rgba_to_bgra(ColorRGBA color){
	return (ColorBGRA){
		.b = color.b,
		.g = color.g,
		.r = color.r,
		.a = color.a
	};
}
