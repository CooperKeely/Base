///////////////////////////////////////
/// cjk: Profiling Functions

void profile_begin() { 
	global_profiling_begin = clock(); 
}

void profile_end(Str8 message) {
	global_profiling_end = clock();
	F64 time_spent = (F64)(global_profiling_end - global_profiling_begin) / CLOCKS_PER_SEC;
	str8_printf(stderr, "[PROFILING] %.*s: %.4f(s)\n", Str8VArg(message), time_spent);
}
