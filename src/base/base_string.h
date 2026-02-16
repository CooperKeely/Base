#ifndef BASE_STRING_H
#define BASE_STRING_H

//////////////////////////////////////
/// cjk: String Definition

typedef U32 Str8_MatchFlags;
enum {
	Str8_MatchRightSideSloppy = (1 << 0),
	Str8_MatchCaseInsensitive = (1 << 1),
};

typedef struct {
	U8 *str;
	U64 size;
} Str8;

#define Str8Lit(S) str8((U8 *)(S), sizeof(S) - 1)
#define Str8VArg(s) (U32)(s).size, (s).str

// C string helpers
U64 cstring_length(const char *c);
Str8 cstring_to_str8(const char *c);
char* str8_to_cstring(Arena* arena, Str8 str);

// character helpers
B32 char_is_digit(const U8 c);
B32 char_is_space(const U8 c);
B32 char_is_upper(const U8 c);
B32 char_is_lower(const U8 c);
B32 char_is_alpha(const U8 c);
B32 char_is_slash(const U8 c);
U8 lower_from_char(const U8 c);
U8 upper_from_char(const U8 c);

// String helpers
U8 upper_from_char(U8 chr);
Str8 str8_skip_last_slash(Str8 str);
S64 str8_find_first_char(Str8 str, U8 c);
S64 str8_find_first_digit(Str8 str);
inline U8 str8_get(Str8 str, U64 idx);
Str8 s64_to_str8(Arena* arena, S64 integer);
S64 str8_to_s64(Str8 str);
F32 str8_to_f32(Str8 str);
Str8 str8_trim_whitespace(Str8 str);

// Str8 output
void str8_printf(FILE *file_ptr, const char *format, ...);
Str8 str8_pushf(Arena *arena, const char *format, ...);

// String functions
Str8 str8(U8 *str, U64 length);
Str8 str8_substr(Str8 s1, Rng1U64 range);
Str8 str8_concat(Arena *arena, Str8 s1, Str8 s2);
Str8 str8_copy(Arena *arena, Str8 s1);
Str8 str8_get_slice(Str8 s1, U64 start_idx, U64 length);
B32 str8_cmp(Str8 s1, Str8 s2);
B32 str8_match(Str8 s1, Str8 s2, Str8_MatchFlags flags);

//////////////////////////////////////
/// cjk: String List Definitions

typedef struct Str8Node Str8Node;
struct Str8Node {
	Str8Node *next;
	Str8 string;
};

typedef struct {
	Str8Node *first;
	Str8Node *last;
	U64 count;
	U64 total_size;
} Str8List;

Str8List str8_list();
Str8List* str8_tokenize_list(Arena* arena, Str8 str, Str8 delimiters);
Str8Node* str8_list_push_node(Str8List *list, Str8Node *node);
Str8Node* str8_list_push_node_front(Str8List *list, Str8Node *node);
Str8Node* str8_list_push(Arena *arena, Str8List *list, Str8 string);
Str8Node* str8_list_push_front(Arena *arena, Str8List *list, Str8 string);
Str8 str8_list_get(Str8List* list, U64 idx);


//////////////////////////////////////
/// cjk: String Array Definitions 
typedef struct {
	Str8 *array;
	U64 count;
	U64 capacity;
} Str8Array;

Str8Array str8_array_reserve(Arena *arena, U64 capacity);
Str8Array str8_tokenize_array(Arena *arena, Str8 string, Str8 delimiters);

#endif //BASE_STRING_H
