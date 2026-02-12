#ifndef BASE_ARENA_H
#define BASE_ARENA_H

///////////////////////////////////////
/// cjk: Arena Definitions 

typedef struct {
	U8 *mem_ptr;
	U64 next_free;
	U64 capacity;
} Arena;

typedef struct {
	Arena *arena;
	U64 original_position;
} TempArena;

#define DEFAULT_ARENA_SIZE MB(64)
#define TEMP_ARENA_SIZE KB(4)

#define ArenaPushArray(arena, type, count) 	(type *)arena_push((arena), sizeof(type) * (count))
#define ArenaPushArrayZero(arena, type, count)	(type *)arena_push_zero((arena), sizeof(type) * (count))
#define ArenaPushStruct(arena, type) 		ArenaPushArray(arena, type, (1))
#define ArenaPushStructZero(arena, type) 	ArenaPushArrayZero(arena, type, (1))

#define TempArenaScope(temp_name, arena_ptr)\
  for (TempArena (temp_name) = temp_arena_begin(arena_ptr);\
       (temp_name).arena != NULL;\
       temp_arena_end((temp_name)), (temp_name).arena = NULL)


/* arena helper functions */
U64 arena_get_position(Arena *arena);
Arena *arena_alloc();
Arena *arena_alloc_with_capacity(U64 size);
void arena_release(Arena *arena);
void *arena_push(Arena *arena, U64 size);
void *arena_push_zero(Arena *arena, U64 size);
void arena_pop(Arena *arena, U64 size);
void arena_pop_to(Arena *arena, U64 pos);

TempArena temp_arena_begin(Arena *arena);
void temp_arena_end(TempArena temp);

#endif // BASE_ARENA_H
