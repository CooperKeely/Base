#define BASE_IMPLEMENTATION
#include "Base.h"

int main(void){
	Arena* arena = arena_alloc();

	CSV_Config config = {
		.headers_enabled = TRUE,
		.delimiters = Str8Lit("")
	};

	CSV csv_parser = csv_init(arena, config, "resources/customers-2000000.csv");

	Profile("Iterating through CSV"){
		for EachCSVRow(&csv_parser, row){
			/*
			for EachNode(item, Str8Node, row->list->first){
				str8_printf(stdout, "%.*s", Str8VArg(item->string));
			}*/
		}	
	}

	arena_release(arena);
	return 0;
}
