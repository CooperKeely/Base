//////////////////////////////////////
/// cjk: HashMap Functions 

HashMap *hash_map_init(Arena *arena) {
	HashMap *hashmap = ArenaPushStructZero(arena, HashMap);
	hashmap->arena = arena;
	hashmap->next_free_collision_bucket = U64Lit(0);
	return hashmap;
}

void *hash_map_get(HashMap *hash_map, Str8 key) {
	Assert(hash_map);
	Assert(key.size > 0);
	Assert(key.str);

	U64 hash = hash_fnv1a_u64(key.str, key.size);
	hash = hash % DEFAULT_HASH_MAP_SIZE;

	HashBucket bucket = hash_map->data[hash];

	if (str8_cmp(key, bucket.key)) return bucket.data;

	while (bucket.has_next_data) {
		if (str8_cmp(key, bucket.key)) return bucket.data;
		bucket = hash_map->data[bucket.next_data];
	}
	return NULL;
}

void hash_map_put(HashMap *hash_map, Str8 key, void *data) {
	Assert(hash_map);
	Assert(data);
	Assert(key.size > 0);
	Assert(key.str);

	local_persist U64 collisions = 0;
	U64 hash = hash_fnv1a_u64(key.str, key.size);
	hash = hash % DEFAULT_HASH_MAP_SIZE;

	HashBucket *bucket = &hash_map->data[hash];

	// if bucket is empty
	if (bucket->data == NULL) {
		bucket->data = data;
		bucket->key = str8_copy(hash_map->arena, key);
		bucket->has_next_data = BASE_FALSE;
		bucket->next_data = 0;
		return;
	}

	printf("Collisions: %lu\n", collisions);
	//
	// if this assert goes off consider increasing the size of the hashmap
	// or changing hash function / choosing a different seed
	Assert(collisions < 1000);
	collisions += 1;

	// traverse collision array until empty bucket is found
	HashBucket *prev = NULL;
	while (bucket->data != NULL) {
		prev = bucket;
		if (bucket->has_next_data) {
			bucket = &hash_map->collisions[bucket->next_data];
			continue;
		}
		break;
	}

	// set previous buckets next data to the current bucket
	prev->has_next_data = BASE_TRUE;
	prev->next_data = hash_map->next_free_collision_bucket;

	// insert data
	bucket = &hash_map->collisions[hash_map->next_free_collision_bucket];
	bucket->data = data;
	bucket->key = str8_copy(hash_map->arena, key);
	bucket->has_next_data = BASE_FALSE;
	bucket->next_data = 0;

	hash_map->next_free_collision_bucket += 1;
	Assert(hash_map->next_free_collision_bucket <= DEFAULT_HASH_COLLISION_SIZE);
}

// Hashing functions
U64 hash_fnv1a_u64(const void *in_ptr, U64 num_bytes) {
	Assert(num_bytes > 0);
	Assert(in_ptr);

	const U8 *byte_arr = (const U8 *)in_ptr;

	// seeds for FNV-1a hashing do not touch
	U64 fnv_prime = U64Lit(0x00000100000001b3);
	U64 fnv_hash = U64Lit(0xcbf29ce484222325);

	for EachIndex(i, num_bytes) {
		fnv_hash ^= byte_arr[i];
		fnv_hash *= fnv_prime;
	}

	return fnv_hash;
}
