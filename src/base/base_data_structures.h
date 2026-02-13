#ifndef BASE_DATA_STRUCTURES_H
#define BASE_DATA_STRUCTURES_H


//////////////////////////////////////
/// cjk: Hashing Definitions

#define DEFAULT_HASH_MAP_SIZE KB(4)
#define DEFAULT_HASH_COLLISION_SIZE KB(1)

typedef struct {
	void *data;
	Str8 key;
	U64 next_data;
	B32 has_next_data;
} HashBucket;

typedef struct {
	Arena *arena;
	U64 next_free_collision_bucket;
	HashBucket data[DEFAULT_HASH_MAP_SIZE];
	HashBucket collisions[DEFAULT_HASH_COLLISION_SIZE];
} HashMap;

HashMap *hash_map_init(Arena *arena);
void *hash_map_get(HashMap *hash_map, Str8 key);
void hash_map_put(HashMap *hash_map, Str8 key, void *data);

// Hashing functions
U64 hash_fnv1a_u64(const void *in_ptr, U64 num_bytes);


#endif //BASE_DATA_STRUCTURES_H
