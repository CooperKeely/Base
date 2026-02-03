#define BASE_IMPLEMENTATION
#include "base.h"


void csv_main(){
	Arena* arena = arena_alloc();
	CSV_Config config = {
		.headers_enabled = 1,
		.delimiters = Str8Lit(","),
	};
	
	CSV csv_file = csv_init(arena, config, "resources/customers-2000000.csv");

	for EachCSVRow(&csv_file, row){
		for EachNode(str_node, Str8Node, row->list->first){
			//str8_printf(stdout, "%s", str_node->string);
		}	
	}

	arena_release(arena);
}


void hashtable_main(){
	Arena* arena = arena_alloc();
	HashMap* hash_map = hash_map_init(arena);

	int test_data = 100;
	
	Profile("Hashtable"){
		for EachIndex(i, 5000){
			Str8 key = str8_pushf(arena, "%d", i);
			hash_map_put(hash_map, key, &test_data);
		}
	}
	arena_release(arena);
}


void os_main(){
	os_entry_point(0, NULL);
}



int main(void){
	os_entry_point(0, NULL);
	hashtable_main();
        return 0;
}




