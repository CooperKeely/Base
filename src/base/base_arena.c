

///////////////////////////////////////
/// cjk: Arena Functions

U64 arena_get_position(Arena *arena) {
	Assert(arena != NULL);
	return arena->next_free;
}

Arena *arena_alloc() { 
	return arena_alloc_with_capacity(DEFAULT_ARENA_SIZE); 
}

Arena *arena_alloc_with_capacity(U64 size) {
	Arena *arena = (Arena *)malloc(sizeof(Arena));
	Assert(arena);

	arena->capacity = size;

	arena->mem_ptr = (U8 *)malloc(arena->capacity);
	Assert(arena->mem_ptr);
	AsanPoisonMemoryRegion(arena->mem_ptr, size);

	arena->next_free = 0;
	return arena;
}

void arena_release(Arena *arena) {
	Assert(arena != NULL);
	arena->capacity = 0;
	arena->next_free = 0;
	free(arena->mem_ptr);
	free(arena);
}

void *arena_push(Arena *arena, U64 size) {
	Assert(arena != NULL);
	if (arena->next_free + size <= arena->capacity) {
		void *ptr = arena->mem_ptr + arena->next_free;
		AsanUnpoisonMemoryRegion(ptr, size);
		arena->next_free += size;
		return ptr;
	}
	InvalidPath;
	return NULL;
}

void *arena_push_zero(Arena *arena, U64 size) {
	Assert(arena != NULL);
	if (arena->next_free + size <= arena->capacity) {
		void *ptr = arena->mem_ptr + arena->next_free;
		AsanUnpoisonMemoryRegion(ptr, size);
		arena->next_free += size;
		memset(ptr, 0, size);
		return ptr;
	}
	InvalidPath;
	return NULL;
}

void arena_pop(Arena *arena, U64 size) {
	Assert(arena != NULL);
	U64 pos = arena->next_free - size;
	arena_pop_to(arena, pos);
}

void arena_pop_to(Arena *arena, U64 new_pos) {
	Assert(arena);
	Assert(arena->mem_ptr);

	U64 current_pos = arena_get_position(arena);

	Assert(new_pos <= current_pos);

	U64 asan_poison_size = current_pos - new_pos;
	void *asan_poison_ptr = arena->mem_ptr + new_pos;
	AsanPoisonMemoryRegion(asan_poison_ptr, asan_poison_size);

	arena->next_free = new_pos;
}

TempArena temp_arena_begin(Arena *arena) {
	Assert(arena != NULL);
	TempArena temp = {arena, arena_get_position(arena)};
	return temp; 
}

void temp_arena_end(TempArena temp) {
	Assert(temp.arena != NULL);
	arena_pop_to(temp.arena, temp.original_position);
}
