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



void line_draw_test(WM_Context ctx){
		// --- Group 1: Orthogonal (Top Left) ---
		wm_draw_line(&ctx, Vec2_U32(50, 50), Vec2_U32(150, 50),  COLOR_WHITE); // Horizontal
		wm_draw_line(&ctx, Vec2_U32(50, 50), Vec2_U32(50, 150),  COLOR_WHITE); // Vertical

		// --- Group 2: 45 Degree Diagonals (Top Middle) ---
		wm_draw_line(&ctx, Vec2_U32(300, 50),  Vec2_U32(400, 150), COLOR_WHITE); // Down-Right
		wm_draw_line(&ctx, Vec2_U32(450, 150), Vec2_U32(550, 50),  COLOR_WHITE); // Up-Right
		wm_draw_line(&ctx, Vec2_U32(700, 150), Vec2_U32(600, 50),  COLOR_WHITE); // Up-Left
		wm_draw_line(&ctx, Vec2_U32(850, 50),  Vec2_U32(750, 150), COLOR_WHITE); // Down-Left

		// --- Group 3: Shallow Lines |dx| > |dy| (Center Row) ---
		// Rightward shallow
		wm_draw_line(&ctx, Vec2_U32(50, 300),  Vec2_U32(250, 350), COLOR_GREEN); // Octant 0
		wm_draw_line(&ctx, Vec2_U32(250, 370), Vec2_U32(50, 320),  COLOR_GREEN); // Octant 4 (Reverse)
		// Leftward shallow
		wm_draw_line(&ctx, Vec2_U32(550, 300), Vec2_U32(350, 350), COLOR_YELLOW); 
		wm_draw_line(&ctx, Vec2_U32(350, 370), Vec2_U32(550, 320), COLOR_YELLOW);

		// --- Group 4: Steep Lines |dy| > |dx| (Bottom Row) ---
		// Downward steep
		wm_draw_line(&ctx, Vec2_U32(50,  450), Vec2_U32(100, 650), COLOR_RED);   // Octant 1
		wm_draw_line(&ctx, Vec2_U32(120, 650), Vec2_U32(70,  450), COLOR_RED);   // Octant 5 (Reverse)
		// Upward steep
		wm_draw_line(&ctx, Vec2_U32(250, 650), Vec2_U32(300, 450), COLOR_BLUE);
		wm_draw_line(&ctx, Vec2_U32(320, 450), Vec2_U32(270, 650), COLOR_BLUE);

		// --- Group 5: Boundary/Clipping Test ---
		// These test if your loop handles coordinates outside [0, 1279]
		wm_draw_line(&ctx, Vec2_U32(1200, 10), Vec2_U32(1300, 100), COLOR_RED);  // Right edge clip
		wm_draw_line(&ctx, Vec2_U32(10, 700),  Vec2_U32(100, 800),  COLOR_BLUE); // Bottom edge clip
		wm_draw_line(&ctx, Vec2_U32(-50, 360), Vec2_U32(50, 360),   COLOR_GREEN); // Left edge clip
}


void triangle_draw_test(WM_Context ctx){
	
	// 1. Degenerate Case: A Single Point
	// Should draw nothing or just one pixel.
	wm_draw_triangle(&ctx, Vec2_U32(100, 100), Vec2_U32(100, 100), Vec2_U32(100, 100), COLOR_RED); 

	// 2. Degenerate Case: A Flat Line
	// Three collinear points. Should look like a single horizontal line.
	wm_draw_triangle(&ctx, Vec2_U32(10, 10), Vec2_U32(50, 10), Vec2_U32(100, 10), COLOR_GREEN);

	// 3. Standard Case: Right-angled Triangle
	// Perfect for checking if the fill/lines align on axes.
	wm_draw_triangle(&ctx, Vec2_U32(200, 200), Vec2_U32(200, 300), Vec2_U32(300, 300), COLOR_BLUE);

	// 4. "The Needle": Very thin triangle
	// Tests the precision of your Bresenham/Rasterizer logic.
	wm_draw_triangle(&ctx, Vec2_U32(400, 100), Vec2_U32(405, 100), Vec2_U32(600, 500), COLOR_YELLOW);

	// 5. Clipping Case: Partially off-screen (Left/Top)
	// One or more vertices have coordinates near or at 0.
	wm_draw_triangle(&ctx, Vec2_U32(-10, -10), Vec2_U32(50, 10), Vec2_U32(10, 50), COLOR_MAGENTA);

	// 6. Clipping Case: Partially off-screen (Right/Bottom)
	// One or more vertices exceed ctx.size.width/height.
	U32 w = ctx.size.width;
	U32 h = ctx.size.height;
	wm_draw_triangle(&ctx, Vec2_U32(w-20, h-20), Vec2_U32(w+50, h-20), Vec2_U32(w-20, h+50), COLOR_CYAN);

	// 7. Winding Order Test
	// If your algorithm depends on clockwise/counter-clockwise, 
	// try drawing the same triangle with swapped vertices.
	wm_draw_triangle(&ctx, Vec2_U32(500, 500), Vec2_U32(550, 600), Vec2_U32(450, 600), COLOR_WHITE);

}

void x11_graphics(){

	Arena* arena = arena_alloc();

	WM_Context ctx = wm_open_window(arena, 
				 Rect1_U16(100,100,1280,720), 
				 Str8Lit("Demo"),
				 0,
				 RGBA(0,0,0,0),
				 RGBA(0, 0, 0, 0));

	wm_register_input_events(&ctx, WM_Event_Keyboard_KeyPress
			  |WM_Event_Keyboard_KeyRelease
			  |WM_Event_WindowState_ConfigureNotify);

	volatile B32 quit = false;
	



	while(!quit){
		Profile("Frame"){
			while(wm_num_of_pending_events(&ctx) > 0){
				XEvent event = {0};
				XNextEvent(ctx.display, &event);

				if(event.type == KeyPress){
					wm_resize_window(&ctx, 100, 100);
					quit = true;
				}else if(event.type == ConfigureNotify){
					XConfigureEvent xce = event.xconfigure;
					ctx.size.width = (U16) xce.width;
					ctx.size.height = (U16) xce.height;
				}
			}	
			
			
			
			// Background
			wm_draw_rect(&ctx, Rect1_U32(0, 0, ctx.size.width, ctx.size.height), RGBA(0,0,0,0));
		
			// 2d primitive testing
			triangle_draw_test(ctx);

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




