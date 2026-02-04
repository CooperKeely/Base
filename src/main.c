#include <X11/X.h>
#define BASE_IMPLEMENTATION
#define BASE_ENABLE_WINDOW 
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

void x11_graphics(){

	Arena* arena = arena_alloc();

	WM_Context ctx = wm_open_window(arena, 
				 (RectU16){100,100,800,600}, 
				 Str8Lit("Demo"),
				 0,
				 (ColorRGBA){0, 0, 0, 0},
				 (ColorRGBA){0, 0, 0, 0});

	wm_register_input_events(&ctx, WM_Event_Keyboard_KeyPress|WM_Event_Keyboard_KeyRelease);

	volatile B32 quit = false;

	while(!quit){
		Profile("Frame"){
			while(wm_num_of_pending_events(&ctx) > 0){
				XEvent event = {0};
				XNextEvent(ctx.display, &event);

				if(event.type == KeyPress){
					wm_resize_window(&ctx, 100, 100);
					quit = true;
				}
			}	
			wm_draw_rect(&ctx, (RectU16){100, 100, 700, 500}, (ColorRGBA){0, 100, 0, 0});
			wm_draw_rect(&ctx, (RectU16){200, 200, 300, 500}, (ColorRGBA){0, 0, 100, 0});
			wm_draw_window(&ctx);
		}
		
	}

	wm_close_window(&ctx);
	arena_release(arena);
}


void os_main(){
	os_entry_point(0, NULL);
}



int main(void){
	os_entry_point(0, NULL);
	x11_graphics();

        return 0;
}




