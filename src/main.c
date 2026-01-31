#define BASE_IMPLEMENTATION
#include "base.h"

int main(void){
	Arena* arena = arena_alloc();
	CSV_Config config = {
		.headers_enabled = 1,
		.delimiters = Str8Lit(","),
	};
	
	CSV csv_file = csv_init(arena, config, "../resources/customers-2000000.csv");

	for EachCSVRow(&csv_file, row){
		for EachNode(str_node, Str8Node, row->list->first){
			str8_printf(stdout, "%s", str_node->string);
		}	
	}

	arena_release(arena);

	return 0;
}




