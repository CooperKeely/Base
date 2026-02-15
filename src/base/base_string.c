///////////////////////////////////////
/// cjk: String Functions 

U8 upper_from_char(U8 chr) {
	if (chr > 'A') {
		return chr - 32;
	}
	return chr;
}

Str8 str8(U8 *str, U64 length) {
	Assert(str != NULL);
	return (Str8){str, length};
}

void str8_printf(FILE *file_ptr, const char *format, ...) {
	Assert(file_ptr != NULL);
	Assert(format != NULL);
	va_list args;
	va_start(args, format);
	vfprintf(file_ptr, format, args);
	va_end(args);
}

Str8 str8_pushf(Arena *arena, const char *format, ...) {
	Assert(arena);
	Assert(format);

	va_list args;
	va_start(args, format);

	va_list copy;
	va_copy(copy, args);

	U64 len = vsnprintf(NULL, 0, format, copy);

	U8 *ptr = ArenaPushArray(arena, U8, len + 1);
	vsnprintf((char *)ptr, len + 1, format, args);
	arena_pop(arena, 1);

	va_end(args);
	return (Str8){ptr, len};
}

Str8 str8_skip_last_slash(Str8 str) {
	Assert(str.size > 0);
	Assert(str.str);

	U8 *ptr = str.str + str.size - 1;
	for (; ptr >= str.str; ptr -= 1) {
		if (*ptr == '/' || *ptr == '\\') break;
	}

	if (ptr >= str.str) {
		ptr += 1;
		str.size = (U64)(str.str + str.size - ptr);
		str.str = ptr;
	}
	return str;
}

U8 str8_get(Str8 string, U64 idx) {
	if (idx < string.size) {
		return string.str[idx];
	}
	InvalidPath;
	return '\0';
}




Str8 s64_to_str8(Arena* arena, S64 integer){
	if(integer == 0) return Str8Lit("0");
	B32 is_negative = (integer < 0);
	
	U64 abs_val = is_negative ? (U64)(-integer) : (U64)integer;

	U64 digits = 0;
	U64 divide_by = 1;
	for(int i = abs_val; i > 0; i /= 10) {
		digits++; 
		if(i >= 10) divide_by *= 10;
	}

	if(is_negative) digits ++;
	Str8 result = (Str8){
		.str = ArenaPushArray(arena, U8, digits),
		.size = digits 
	};

	if(is_negative) result.str[0] = '-';

	for EachIndex(i, result.size){
		if(is_negative && i == 0) continue;
		U64 digit = (abs_val/divide_by) % 10;
		result.str[i] = (char)(digit + 48);
		divide_by /= 10;
	}	

	return result;
}

U64 cstring_length(const char *c) {
	Assert(c);
	U64 length = 0;
	if (c) {
		U8 *p = (U8 *)c;
		for (; *p != 0; p += 1);
		length = (U64)(p - (U8 *)c);
	}
	return length;
}

Str8 cstring_to_str8(const char *c) {
	Assert(c);
	Str8 ret = {(U8*)c, cstring_length(c)};
	return ret; 
}

char* str8_to_cstring(Arena* arena, Str8 str){
	Assert(arena);
	Assert(str.size > 0);
	Assert(str.str);

	char* cstring = ArenaPushArray(arena, char,str.size + 1);
	MemoryCopy(cstring, str.str, str.size);
	cstring[str.size] = '\0';
	return cstring;	
}

S64 str8_find_first_char(Str8 str, U8 c){
	for EachIndex(idx, str.size){
		if (str8_get(str, idx) == c) return idx;
	}
	return -1;
}

Str8 str8_concat(Arena *arena, Str8 s1, Str8 s2) {
	Assert(arena);

	U64 new_size = s1.size + s2.size;
	U8 *character_buffer = ArenaPushArray(arena, U8, new_size);

	MemoryCopy(character_buffer, s1.str, s1.size);
	MemoryCopy(character_buffer + s1.size, s2.str, s2.size);

	return (Str8){ .str = character_buffer, .size = new_size};
}

Str8 str8_copy(Arena* arena, Str8 str){
	Assert(arena);

	Str8 result = {0};
	result.size = str.size;
	result.str = ArenaPushArray(arena, U8, str.size);
	MemoryCopyStr8(result, str);
	return result;
}

Str8 str8_substr(Str8 s1, Rng1U64 range) {
	range.min = ClampTop(range.min, s1.size);
	range.max = ClampTop(range.max, s1.size);

	s1.str += range.min;
	s1.size = dim_r1u64(range);
	return s1;
}

Str8 str8_get_slice(Str8 s1, U64 start_idx, U64 size){
	Assert(s1.str);
	Assert(start_idx + size <= s1.size);
	
	Str8 string_slice;
	string_slice.str = &s1.str[start_idx];
	string_slice.size = size;
	
	return string_slice;
}

B32 str8_cmp(Str8 s1, Str8 s2) {
	Assert(s1.size > 0);
	Assert(s2.size > 0);
	Assert(s1.str);
	Assert(s2.str);

	if (s1.size != s2.size) return false;
	return MemoryMatch(s1.str, s2.str, s1.size);
}

B32 str8_match(Str8 s1, Str8 s2, Str8_MatchFlags flags) {
	B32 result = 1;

	if (s1.size == s2.size && (flags == 0)) {
		return MemoryMatch(s1.str, s2.str, s1.size);
	}

	else if (s1.size == s2.size || (flags & Str8_MatchRightSideSloppy)) {

		B32 case_insensitive = (flags & Str8_MatchCaseInsensitive);
		U64 size = Min(s1.size, s2.size);

		for EachIndex(i, size) {
			U8 s1_char = s1.str[i];
			U8 s2_char = s2.str[i];

			if (case_insensitive) {
				s1_char = upper_from_char(s1_char);
				s2_char = upper_from_char(s2_char);
			}

			if (s1_char != s2_char) {
				result = 0;
				break;
			}
		}
	}
	return result;
}

///////////////////////////////////////
/// cjk: String List Functions 

Str8List str8_list() {
	return (Str8List){0};
}

Str8Node *str8_list_push_node(Str8List *list, Str8Node *node) {
	Assert(list != NULL);
	Assert(node != NULL);

	if (list->last == NULL && list->first == NULL) {
		list->last = node;
		list->first = node;
	} else {
		list->last->next = node;
		list->last = node;
	}

	list->count += 1;
	list->total_size += node->string.size;
	return node;
}

Str8Node *str8_list_push_node_front(Str8List *list, Str8Node *node) {
	Assert(list != NULL);
	Assert(node != NULL);

	if (list->last == NULL && list->first == NULL) {
		list->last = node;
		list->first = node;
	} else {
		node->next = list->first;
		list->first = node;
	}

	list->count += 1;
	list->total_size += node->string.size;

	return node;
}

Str8Node *str8_list_push(Arena *arena, Str8List *list, Str8 string) {
	Assert(arena != NULL);
	Assert(list != NULL);

	Str8Node *node = ArenaPushStruct(arena, Str8Node);
	*node = (Str8Node){.next = NULL, .string = string};

	return str8_list_push_node(list, node);
}

Str8Node *str8_list_push_front(Arena *arena, Str8List *list, Str8 string) {
	Assert(arena != NULL);
	Assert(list != NULL);

	Str8Node *node = ArenaPushStruct(arena, Str8Node);
	*node = (Str8Node){.next = NULL, .string = string};

	return str8_list_push_node_front(list, node);
}

Str8List *str8_tokenize_list(Arena *arena, Str8 string, Str8 delimiters) {
	Assert(arena != NULL);
	Assert(delimiters.size > 0);

	U64 start_idx = 0;
	Str8List *list = ArenaPushStruct(arena, Str8List);

	for EachIndex(i, string.size) {
		B32 is_delimiter = 0;

		for EachIndex(delim, delimiters.size) {
			if (str8_get(string, i) == str8_get(delimiters, delim)) {
				is_delimiter = 1;
				break;
			}
		}

		if (is_delimiter) {
			Str8 token = str8_substr(string, Rng1_U64(start_idx, i));
			start_idx = i + 1;
			str8_list_push(arena, list, token);
		}
	}

	if (start_idx <= string.size) { // Push final token
		Str8 token = str8_substr(string, Rng1_U64(start_idx, string.size));
		str8_list_push(arena, list, token);
	}

	return list;
}

///////////////////////////////////////
/// cjk: String Array Functions 

Str8Array str8_array_reserve(Arena *arena, U64 capacity){
	NotImplemented;
}
Str8Array str8_tokenize_array(Arena *arena, Str8 string, Str8 delimiters){
	NotImplemented;
}
