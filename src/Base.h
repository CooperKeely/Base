#ifndef BASE_H
#define BASE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>



///////////////////////////////////////
/// cjk: Macro Magic 

#define DeferLoop(begin, end) for(int _i_ = ((begin), 0); _i_ != 1 ; _i_ += 1 , (end))

#if defined(__clang__) || defined(__GNUC__)
	#define Trap() __builtin_trap()
	#define Likely(x) __builtin_expect(!!(x), 1)
	#define Unlikely(x) __builtin_expect(!!(x), 0)
	#define StaticAssert(c, label) _Static_assert(c, #label)
#endif

// For loop helpers
#define EachInRange(idx, range) (U64 idx = (range).min; idx < (range).max; idx++)
#define EachIndex(idx, count) (U64 idx = 0; idx < (count); idx++)
#define EachNode(idx, T, first) (T* idx = first; idx != NULL; idx = idx->next)



///////////////////////////////////////
/// cjk: Helper Macros + Types

#define Min(A,B) (((A)<(B))?(A):(B))
#define Max(A,B) (((A)>(B))?(A):(B)) 

#define ClampMax(A,B) Min(A,B)
#define ClampMin(A,B) Max(A,B)

#define KB(n) (((U64)(n)) << 10)
#define MB(n) (((U64)(n)) << 20)
#define GB(n) (((U64)(n)) << 30)

#define Thousand(n) ((n) * 1000)
#define Million(n) ((n) * Thousand(1000))
#define Billion(n) ((n) * Million(1000))

#define R1U32(min, max)  rng_1u32((min), (max))
#define R1F32(min, max)  rng_1f32((min), (max))
#define R1U64(min, max)  rng_1u64((min), (max))
#define R1F64(min, max)  rng_1f64((min), (max))

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

typedef float F32;
typedef double F64;

typedef enum{
	FALSE = 0,
	TRUE = 1
} Bool;

// Vector Types
typedef struct{
	U32 x;
	U32 y;
} Vec2U32;

typedef struct{
	F32 x;
	F32 y;
} Vec2F32;

typedef struct{
	U32 x;
	U32 y;
	U32 z;
} Vec3U32;

typedef struct{
	F32 x;
	F32 y;
	F32 z;
} Vec3F32;

// Range Type
typedef union {
	struct{
		U32 min;
		U32 max;
	};
	U32 v[2];
} Rng1U32;

typedef union {
	struct{
		F32 min;
		F32 max;
	};
	F32 v[2];
} Rng1F32;

typedef union {
	struct{
		U64 min;
		U64 max;
	};
	U64 v[2];
} Rng1U64;

typedef union {
	struct{
		F64 min;
		F64 max;
	};
	F64 v[2];
} Rng1F64;


Rng1U32 rng_1u32(U32 min, U32 max);

Rng1F32 rng_1f32(F32 min, F32 max);

Rng1U64 rng_1u64(U64 min, U64 max);
U64 dim_r1u64(Rng1U64 range);

Rng1F64 rng_1f64(F64 min, F64 max);



///////////////////////////////////////
/// cjk: Memory Operations Macros 

#define MemoryCopy(dest, src, num_bytes) memmove((dest), (src), (num_bytes))
#define MemorySet(dest, byte, num_bytes) memset((dest), (byte), (num_bytes))



///////////////////////////////////////
/// cjk: Error Handeling 

#define STACK_TRACE_SIZE 10

#define Assert(condition, message) \
	do { \
		if (Unlikely(!(condition))) { \
			    fprintf(stderr, "\033[1;31m\n[ASSERTION FAILED]: %s\033[0m\n", message); \
			    fprintf(stderr, "\033[1mExpression:\033[0m %s\n", #condition); \
			    fprintf(stderr, "\033[1mLocation:\033[0m   %s:%d\n", __FILE__, __LINE__); \
			    print_stack_trace(); \
			    Trap(); \
		} \
    	} while(0)

#define AssertNotNull(var)\
	do { \
		if (Unlikely((var) == NULL)) { \
			    fprintf(stderr, "\033[1;31m\n[ASSERT NOT NULL FAILED]: %s\033[0m\n", #var); \
			    fprintf(stderr, "\033[1mLocation:\033[0m   %s:%d\n", __FILE__, __LINE__); \
			    print_stack_trace(); \
			    Trap(); \
		} \
    	} while(0)

#define Unreachable(message)\
	do { \
		    fprintf(stderr, "\033[1;31m\n[UNREACHABLE CODE PATH]: %s\033[0m\n", message); \
		    fprintf(stderr, "\033[1mLocation:\033[0m   %s:%d\n", __FILE__, __LINE__); \
		    print_stack_trace(); \
		    Trap(); \
	} while(0)


#define Panic(message)\
	do { \
		    fprintf(stderr, "\033[1;31m\n[PANIC]: %s\033[0m\n", message); \
		    fprintf(stderr, "\033[1mLocation:\033[0m   %s:%d\n", __FILE__, __LINE__); \
		    print_stack_trace(); \
		    Trap(); \
	} while(0)

void print_stack_trace(void);



///////////////////////////////////////
/// cjk: Arena Implementation

#define DEFAULT_ARENA_SIZE MB(64)
#define TEMP_ARENA_SIZE KB(4)

#define ArenaPushArray(arena, type, count) (type *)arena_push((arena), sizeof(type)*(count))
#define ArenaPushArrayZero(arena, type, count) (type *)arena_push_zero((arena), sizeof(type)*(count))
#define ArenaPushStruct(arena, type) ArenaPushArray(arena, type, (1))
#define ArenaPushStructZero(arena, type) ArenaPushArrayZero(arena, type, (1))

#define ScratchArenaScope(arena_ptr) for(\
		ScratchArena _scratch_ = scratch_arena_begin(arena_ptr);\
		_scratch_.arena != NULL;\
		scratch_arena_end(_scratch_), _scratch_.arena=NULL) 

#define TempArenaScope(temp_arena_ptr) DeferLoop((Arena* (temp_arena_ptr) = arena_alloc_with_capacity(TEMP_ARENA_SIZE))\
						 ,arena_release((temp_arena_ptr)))
	
typedef struct{
	U8* mem_ptr;
	U64 next_free;
	U64 capacity;
} Arena;

typedef struct{
	Arena* arena;
	U64 original_position;
} ScratchArena;

/* arena helper functions */
U64 arena_get_position(Arena* arena);
Arena* arena_alloc();
Arena* arena_alloc_with_capacity(U64 size);
void arena_release(Arena* arena);
void* arena_push(Arena* arena, U64 size);
void* arena_push_zero(Arena* arena, U64 size);
void arena_pop(Arena* arena, U64 size);
void arena_pop_to(Arena* arena, U64 pos);
ScratchArena scratch_arena_begin(Arena* arena);
void scratch_arena_end(ScratchArena scratch);



///////////////////////////////////////
/// cjk: String Implementation 

#define Str8Lit(S) str8((U8*)(S), sizeof(S)-1)
#define Str8VArg(s) (U32)(s).size, (s).str

typedef struct {
	U8* str;
	U64 size;
} Str8;

Str8 str8(U8* str, U64 length);
void str8_printf(FILE* file_ptr, const char* format, ...);
inline U8 str8_get(Str8 str, U64 idx);
U64 cstring_length(char* c);
Str8 cstring_to_str8(char *c);
Str8 str8_substr(Str8 s1, Rng1U64 range);
Str8 str8_concat(Arena* arena, Str8 s1, Str8 s2);
Str8 str8_copy(Arena* arena, Str8 s1);

// Str8 List
typedef struct Str8Node Str8Node;
struct Str8Node{
	Str8Node* next;
	Str8 string;
};

typedef struct{
	Str8Node* first;
	Str8Node* last;
	U64 count;
	U64 total_size;
} Str8List;

Str8List str8_list();
Str8Node* str8_list_push_node(Str8List* list, Str8Node* node);
Str8Node* str8_list_push_node_front(Str8List* list, Str8Node* node);
Str8Node* str8_list_push(Arena* arena, Str8List* list, Str8 string);
Str8Node* str8_list_push_front(Arena* arena, Str8List* list, Str8 string);

// Str8 array
typedef struct{
	Str8 *array;
	U64 count;
	U64 capacity;
} Str8Array;

Str8Array str8_array_reserve(Arena* arena, U64 capacity);
Str8Array str8_tokenize_array(Arena* arena, Str8 string, Str8 delimiters);



///////////////////////////////////////
/// cjk: Hashing 

#define DEFAULT_HASH_MAP_SIZE KB(4)
#define DEFAULT_HASH_COLLISION_SIZE KB(1)

typedef struct{
	void* data;
	U64 next_data;
} HashBucket;

typedef struct{
	Arena* arena;
	HashBucket data[DEFAULT_HASH_MAP_SIZE];
	HashBucket collisions[DEFAULT_HASH_COLLISION_SIZE];
} HashMap;


HashMap hash_map_init(Arena* arena);
void* hash_map_get(HashMap* hash_map, Str8 key);
void hash_map_put(HashMap* hash_map, Str8 key, void* data);

// Hashing functions
U64 hash_fnv1a_u64(Str8 str);

U64 hash_fnv1a_u64(Str8 str){
	Assert(str.size > 0, "String has no size");
	
	// two 19 digit prime numbers DO NOT TOUCH
	U64 fnv_prime  = 6550123626356161697;
	U64 fnv_hash = 4072063302836454931;
	
	for EachIndex(i, str.size){
		fnv_hash ^= str8_get(str, i);
		fnv_hash *= fnv_prime;	
	}
	return fnv_hash;
}



///////////////////////////////////////
/// cjk: Profiling 

static clock_t global_profiling_begin, global_profiling_end;

#define ProfileFunc() Profile(__func__)
#define Profile(description) DeferLoop(profile_begin(), profile_end(Str8Lit(description)))


void profile_begin();
void profile_end(Str8 description);



///////////////////////////////////////
/// cjk: CSV Parser Implementation

#define EachCSVRow(csv_ptr, row_ptr) (CSV_Row* row_ptr = csv_next_row(csv_ptr); row_ptr != NULL; row_ptr = csv_next_row(csv_ptr))  

typedef struct{
	Str8 delimiters;
	Bool headers_enabled;
} CSV_Config;

typedef struct {
	Str8List* list;
	U64 index;
}CSV_Row;

typedef struct{
	FILE* file_ptr;
	Arena* arena;
	CSV_Config config;
	HashMap column_hash_map;
	CSV_Row current_row;
	ScratchArena row_scratch_arena;
	U8 row_buffer[KB(16)];
	U64 rows;
	U64 columns;
} CSV;

CSV csv_init(Arena* arena, CSV_Config config, char* file_path);
CSV_Row* csv_next_row(CSV* csv);
void csv_row_parse(CSV* csv, Str8 raw_row);



#endif //BASE_H


// Private function definitions
#ifdef BASE_IMPLEMENTATION
#include <execinfo.h>



///////////////////////////////////////
/// cjk: Math Functions 

Rng1U32 rng_1u32(U32 min, U32 max){
	Assert(min < max, "Min is larger than Max");
	return (Rng1U32){
		.min = min,
		.max = max
	};
}

Rng1F32 rng_1f32(F32 min, F32 max){
	Assert(min < max, "Min is larger than Max");
	return (Rng1F32){
		.min = min,
		.max = max
	};
}

Rng1U64 rng_1u64(U64 min, U64 max){
	Assert(min < max, "Min is larger than Max");
	return (Rng1U64){
		.min = min,
		.max = max
	};
}

Rng1F64 rng_1f64(F64 min, F64 max){
	Assert(min < max, "Min is larger than Max");
	return (Rng1F64){
		.min = min,
		.max = max
	};
}


U64 dim_r1u64(Rng1U64 r){
	return ((r.max > r.min) ? (r.max-r.min) : 0);
}



///////////////////////////////////////
/// cjk: Error Handeling Implementation

void print_stack_trace(void){
	void* buffer[STACK_TRACE_SIZE];
	int size = backtrace(buffer, STACK_TRACE_SIZE);
	char** symbols = backtrace_symbols(buffer, size);

	if(symbols == NULL) return;

	fprintf(stderr, "\n\033[1;33m--- STACK TRACE ---\033[0m\n");
   
    	// Start from i= 1 to skip print_stack_trace and the assert macro/caller
    	for EachInRange(i, R1U64(1, size-3)) {
		// Highlighting the function name and address
		fprintf(stderr, " \033[1;30m[%ld]\033[0m %s\n", i - 1, symbols[i]);
    	}

        fprintf(stderr, "\033[1;33m-------------------\033[0m\n\n");
}



///////////////////////////////////////
/// cjk: arena implementation

U64 arena_get_position(Arena* arena){
	AssertNotNull(arena);
	return arena->next_free;
}

Arena* arena_alloc(){
	return arena_alloc_with_capacity(DEFAULT_ARENA_SIZE);
}

Arena* arena_alloc_with_capacity(U64 size){
	Arena* arena = (Arena*) malloc(sizeof(Arena));
	Assert(arena,"memory allocation failed");

	arena->capacity = size;

	arena->mem_ptr = (U8*) malloc(arena->capacity);
	Assert(arena->mem_ptr,"memory allocation failed");
	
	arena->next_free = 0;
	return arena;

}

void arena_release(Arena* arena){
	AssertNotNull(arena);
	arena->capacity = 0;
	arena->next_free = 0;
	free(arena->mem_ptr);
	free(arena);
}

void* arena_push(Arena* arena, U64 size){
	AssertNotNull(arena);
	if(arena->next_free + size <= arena->capacity){
		void* ptr = arena->mem_ptr + arena->next_free;
		arena->next_free += size;
		return ptr;
	}
	Panic("Arena full");
	return NULL;
}

void* arena_push_zero(Arena* arena, U64 size){
	Assert(arena,"Arena is null");
	if(arena->next_free + size <= arena->capacity){
		void* ptr = arena->mem_ptr + arena->next_free;
		arena->next_free += size;
		memset(ptr, 0, size);
		return ptr;
	}
	Panic("Arena full");
	return NULL;
}

void arena_pop(Arena* arena, U64 size){
	AssertNotNull(arena);
	U64 pos = arena->next_free - size;
	arena_pop_to(arena, pos);
	Panic("Tried to remove from an empty arena");
}

void arena_pop_to(Arena* arena, U64 pos){
	AssertNotNull(arena);
	Assert(pos < arena_get_position(arena), "Position is outside of current arena");
	
	arena->next_free = pos;
}

ScratchArena scratch_arena_begin(Arena* arena){
	AssertNotNull(arena);
	return (ScratchArena){
		.arena = arena,
		.original_position = arena_get_position(arena)
	};
}

void scratch_arena_end(ScratchArena scratch){
	AssertNotNull(scratch.arena);
	arena_pop_to(scratch.arena, scratch.original_position);
}



///////////////////////////////////////
/// cjk: String Implementation 

Str8 str8(U8* str, U64 length){
	AssertNotNull(str);
	return (Str8) {str, length};
}

void str8_printf(FILE* file_ptr, const char* format, ...){
	AssertNotNull(file_ptr);
	AssertNotNull(format);
	va_list args;
	va_start(args, format);
	vfprintf(file_ptr, format, args);
	va_end(args);
}

U8 str8_get(Str8 string, U64 idx){
	if(idx < string.size){
		return string.str[idx];
	}	
	Panic("Out of bounds string access");
	return '\0';
}

U64 cstring_length(char* c){
	AssertNotNull(c);
	U64 length = 0;
	if(c){
		U8 *p = (U8*)c;	
		for(; *p != 0; p += 1);
		length = (U64)(p - (U8*) c);
	}
	return length;
}

Str8 cstring_to_str8(char *c){
	AssertNotNull(c);
	return (Str8){
		(U8*) c,
		cstring_length(c),
	};
}


Str8 str8_concat(Arena* arena, Str8 s1, Str8 s2){
	AssertNotNull(arena);

	U64 new_size = s1.size + s2.size;
	U8* character_buffer = ArenaPushArray(arena, U8, new_size);
	
	MemoryCopy(character_buffer, s1.str, s1.size);
	MemoryCopy(character_buffer + s1.size, s2.str, s2.size);

	return (Str8) {
		.str = character_buffer,
		.size = new_size,
	};
}

Str8 str8_copy(Arena* arena, Str8 s1){
	AssertNotNull(arena);

	Str8 new_string = (Str8){
		.str = ArenaPushArray(arena, U8, s1.size),
		.size = s1.size,
	};

	MemoryCopy(new_string.str, s1.str, s1.size);
	return new_string;
}

Str8 str8_substr(Str8 s1, Rng1U64 range){
	range.min = ClampMax(range.min, s1.size);
	range.max = ClampMax(range.max, s1.size);

	s1.str += range.min;
	s1.size = dim_r1u64(range);
	return s1;
}

Str8List str8_list(){
	return (Str8List){
		.first = NULL,
		.last = NULL,
		.count = 0,
		.total_size = 0,
	};
}

Str8Node* str8_list_push_node(Str8List* list, Str8Node* node){
	AssertNotNull(list);
	AssertNotNull(node);
	
	if(list->last == NULL && list->first == NULL){
		list->last = node;
		list->first = node;
	}else{
		list->last->next = node;
		list->last = node;
	}

	list->count += 1;
	list->total_size += node->string.size;
	return node;
}

Str8Node* str8_list_push_node_front(Str8List* list, Str8Node* node){
	AssertNotNull(list);
	AssertNotNull(node);
	
	if(list->last == NULL && list->first == NULL){
		list->last = node;
		list->first = node;
	}else{
		node->next = list->first;
		list->first = node;	
	}

	list->count += 1;
	list->total_size += node->string.size;

	return node;
}

Str8Node* str8_list_push(Arena* arena, Str8List* list, Str8 string){
	AssertNotNull(arena);
	AssertNotNull(list);


	Str8Node* node = ArenaPushStruct(arena, Str8Node);
	*node = (Str8Node){.next=NULL, .string=string};

	return str8_list_push_node(list, node);
}

Str8Node* str8_list_push_front(Arena* arena, Str8List* list, Str8 string){
	AssertNotNull(arena);
	AssertNotNull(list);

	Str8Node* node = ArenaPushStruct(arena, Str8Node);
	*node = (Str8Node) {.next=NULL, .string=string};

	return str8_list_push_node_front(list, node);
}

Str8List* str8_tokenize_list(Arena* arena, Str8 string, Str8 delimiters){
	AssertNotNull(arena);
	Assert(delimiters.size > 0, "No delimiters passed");
	
	U64 start_idx = 0;
	Str8List* list = ArenaPushStruct(arena, Str8List);

	for EachIndex(i, string.size){
		Bool is_delimiter = FALSE;	

		for EachIndex(delim, delimiters.size){
			if(str8_get(string, i) == str8_get(delimiters, delim)){
				is_delimiter = TRUE;
				break;
			}
		}

		if(is_delimiter){
			Str8 token = str8_substr(string, R1U64(start_idx, i));
			start_idx = i + 1;
			str8_list_push(arena, list, token);
		}
	}

	if(start_idx <= string.size){ // Push final token
		Str8 token = str8_substr(string, R1U64(start_idx, string.size));
		str8_list_push(arena, list, token);
	}	

	return list;
}



///////////////////////////////////////
/// cjk: Profiling 

void profile_begin(){
	global_profiling_begin = clock();	
}

void profile_end(Str8 message){
	global_profiling_end = clock();
	F64 time_spent = (F64)(global_profiling_end - global_profiling_begin)/CLOCKS_PER_SEC;
	str8_printf(stderr, "[PROFILING] %.*s: %.4f(s)\n", Str8VArg(message), time_spent);
}



///////////////////////////////////////
/// cjk: CSV Parser Implementation

CSV csv_init(Arena* arena, CSV_Config config, char* file_path){
	AssertNotNull(arena);
	AssertNotNull(file_path);

	CSV csv_parser = {
		.file_ptr = fopen(file_path, "rb"),
		.config = config,
		.arena = arena,
		.column_hash_map= {0},
		.current_row = {0}, 
		.rows = 0,
		.columns = 0
	};

	Assert(csv_parser.file_ptr != NULL, "Failed to open file");

	return csv_parser;
}

CSV_Row* csv_next_row(CSV* csv){
	AssertNotNull(csv);
	
	char* err = fgets((char*) csv->row_buffer, sizeof(csv->row_buffer) , csv->file_ptr);
	if(err == NULL) return NULL;

	Str8 raw_row = cstring_to_str8((char*) csv->row_buffer);
	csv_row_parse(csv, raw_row);	

	return &csv->current_row;
}

void csv_row_parse(CSV* csv, Str8 raw_row){
	AssertNotNull(csv);

	U64 start_idx = 0;
	Str8 delimiters = csv->config.delimiters;
	Str8 quotes = Str8Lit("\"\'");
	
	ScratchArenaScope(csv->arena){
		csv->current_row.list = ArenaPushStructZero(csv->arena, Str8List);
		*csv->current_row.list = str8_list();

		Bool is_quoted = FALSE;

		for EachIndex(i, raw_row.size){
			U8 current_char = str8_get(raw_row, i);
			
			Bool is_quote_char = FALSE;
			for EachIndex(quote, quotes.size){
				if(current_char == str8_get(quotes, quote)){
					is_quote_char = TRUE;
					break;
				}
			}
			
			if(is_quote_char) {
				is_quoted = !is_quoted;
				continue;
			}

			if(is_quoted) break;

			Bool is_delimiter = FALSE;	
			for EachIndex(delim, delimiters.size){
				if(current_char == str8_get(delimiters, delim)){
					is_delimiter = TRUE;
					break;
				}
			}

			if(is_delimiter){
				Str8 token = str8_substr(raw_row, R1U64(start_idx, i));
				str8_list_push(csv->arena, csv->current_row.list, token);
				start_idx = i + 1;
			}
		}

		if(start_idx <= raw_row.size){ // Push final token
			Str8 token = str8_substr(raw_row, R1U64(start_idx, raw_row.size));
			str8_list_push(csv->arena, csv->current_row.list, token);
		}	
	}
}


#endif //BASE_IMPLEMENTATION
