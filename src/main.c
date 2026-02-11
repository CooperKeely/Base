#define BASE_IMPLEMENTATION
#define BASE_ENABLE_WINDOW 
#define BASE_ENABLE_OS

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
		wm_draw_line(&ctx, Vec2_F32(50, 50), Vec2_F32(150, 50),  COLOR_WHITE); // Horizontal
		wm_draw_line(&ctx, Vec2_F32(50, 50), Vec2_F32(50, 150),  COLOR_WHITE); // Vertical

		// --- Group 2: 45 Degree Diagonals (Top Middle) ---
		wm_draw_line(&ctx, Vec2_F32(300, 50),  Vec2_F32(400, 150), COLOR_WHITE); // Down-Right
		wm_draw_line(&ctx, Vec2_F32(450, 150), Vec2_F32(550, 50),  COLOR_WHITE); // Up-Right
		wm_draw_line(&ctx, Vec2_F32(700, 150), Vec2_F32(600, 50),  COLOR_WHITE); // Up-Left
		wm_draw_line(&ctx, Vec2_F32(850, 50),  Vec2_F32(750, 150), COLOR_WHITE); // Down-Left

		// --- Group 3: Shallow Lines |dx| > |dy| (Center Row) ---
		// Rightward shallow
		wm_draw_line(&ctx, Vec2_F32(50, 300),  Vec2_F32(250, 350), COLOR_GREEN); // Octant 0
		wm_draw_line(&ctx, Vec2_F32(250, 370), Vec2_F32(50, 320),  COLOR_GREEN); // Octant 4 (Reverse)
		// Leftward shallow
		wm_draw_line(&ctx, Vec2_F32(550, 300), Vec2_F32(350, 350), COLOR_YELLOW); 
		wm_draw_line(&ctx, Vec2_F32(350, 370), Vec2_F32(550, 320), COLOR_YELLOW);

		// --- Group 4: Steep Lines |dy| > |dx| (Bottom Row) ---
		// Downward steep
		wm_draw_line(&ctx, Vec2_F32(50,  450), Vec2_F32(100, 650), COLOR_RED);   // Octant 1
		wm_draw_line(&ctx, Vec2_F32(120, 650), Vec2_F32(70,  450), COLOR_RED);   // Octant 5 (Reverse)
		// Upward steep
		wm_draw_line(&ctx, Vec2_F32(250, 650), Vec2_F32(300, 450), COLOR_BLUE);
		wm_draw_line(&ctx, Vec2_F32(320, 450), Vec2_F32(270, 650), COLOR_BLUE);

		// --- Group 5: Boundary/Clipping Test ---
		// These test if your loop handles coordinates outside [0, 1279]
		wm_draw_line(&ctx, Vec2_F32(1200, 10), Vec2_F32(1300, 100), COLOR_RED);  // Right edge clip
		wm_draw_line(&ctx, Vec2_F32(10, 700),  Vec2_F32(100, 800),  COLOR_BLUE); // Bottom edge clip
		wm_draw_line(&ctx, Vec2_F32(-50, 360), Vec2_F32(50, 360),   COLOR_GREEN); // Left edge clip
}


void triangle_draw_test(WM_Context ctx){
	
	// 1. Degenerate Case: A Single Point
	// Should draw nothing or just one pixel.
	wm_draw_triangle(&ctx, Vec2_F32(100, 100), Vec2_F32(100, 100), Vec2_F32(100, 100), COLOR_RED); 

	// 2. Degenerate Case: A Flat Line
	// Three collinear points. Should look like a single horizontal line.
	wm_draw_triangle(&ctx, Vec2_F32(10, 10), Vec2_F32(50, 10), Vec2_F32(100, 10), COLOR_GREEN);

	// 3. Standard Case: Right-angled Triangle
	// Perfect for checking if the fill/lines align on axes.
	wm_draw_triangle(&ctx, Vec2_F32(200, 200), Vec2_F32(200, 300), Vec2_F32(300, 300), COLOR_BLUE);

	// 4. "The Needle": Very thin triangle
	// Tests the precision of your Bresenham/Rasterizer logic.
	wm_draw_triangle(&ctx, Vec2_F32(400, 100), Vec2_F32(405, 100), Vec2_F32(600, 500), COLOR_YELLOW);

	// 5. Clipping Case: Partially off-screen (Left/Top)
	// One or more vertices have coordinates near or at 0.
	wm_draw_triangle(&ctx, Vec2_F32(-10, -10), Vec2_F32(50, 10), Vec2_F32(10, 50), COLOR_MAGENTA);

	// 6. Clipping Case: Partially off-screen (Right/Bottom)
	// One or more vertices exceed ctx.size.width/height.
	U32 w = ctx.size.width;
	U32 h = ctx.size.height;
	wm_draw_triangle(&ctx, Vec2_F32(w-20, h-20), Vec2_F32(w+50, h-20), Vec2_F32(w-20, h+50), COLOR_CYAN);

	// 7. Winding Order Test
	// If your algorithm depends on clockwise/counter-clockwise, 
	// try drawing the same triangle with swapped vertices.
	wm_draw_triangle(&ctx, Vec2_F32(500, 500), Vec2_F32(550, 600), Vec2_F32(450, 600), COLOR_WHITE);
}

void circle_draw_test(WM_Context ctx){
	U32 w = ctx.size.width;
	U32 h = ctx.size.height;
	Vec2F32 screen_center = Vec2_F32((F32)w / 2.0f, (F32)h / 2.0f );
	
	// 1. Large Radius: Mostly off-screen
	// Tests if your loop handles being 90% clipped without lagging.
	wm_draw_circle(&ctx, screen_center, (F32)w * 0.8f, COLOR_BLACK);

	// 2. Standard Case: Concentric Circles
	// This tests if the radius is being calculated consistently.
	// If they overlap or look "wobbly," your error term logic is off.
	wm_draw_circle(&ctx, screen_center, 150.0f, COLOR_BLUE);
	wm_draw_circle(&ctx, screen_center, 100.0f, COLOR_GREEN);
	wm_draw_circle(&ctx, screen_center, 50.0f,  COLOR_RED);

	// 3. The "Dot": Minimum Radius
	// Should draw a single pixel or a tiny 2x2/3x3 cluster.
	wm_draw_circle(&ctx, Vec2_F32(20.0f, 20.0f), 1.0f, COLOR_YELLOW);

	// 4. Edge Clipping: Center is off-screen
	// Only a "bow" or arc should be visible.
	wm_draw_circle(&ctx, Vec2_F32(0.0f, 0.0f), 50.0f, COLOR_MAGENTA);

	// 5. Side Clipping: Circle intersects the right/bottom boundaries
	// This is where many algorithms crash due to array out-of-bounds.
	wm_draw_circle(&ctx, Vec2_F32(w, h), 60.0f, COLOR_CYAN);

	// 6. Floating Point Precision: Tiny offset
	// Draw two circles with a 0.5 pixel difference. 
	// If they look identical, your algorithm is casting to int too early.
	wm_draw_circle(&ctx, Vec2_F32(150.0f, 150.0f), 30.0f, COLOR_WHITE);
	wm_draw_circle(&ctx, Vec2_F32(150.5f, 150.5f), 30.0f, COLOR_RED);	
}

void rect_draw_test(WM_Context ctx) {
    U32 w = ctx.size.width;
    U32 h = ctx.size.height;

    // 1. Large Fill (Stress Test)
    wm_draw_rect(&ctx, Rect_F32(0.0f, 0.0f, (F32)w, (F32)h), COLOR_CYAN);

    // 2. Standard Case: A simple centered rectangle
    wm_draw_rect(&ctx, Rect_F32(100.0f, 100.0f, 200.0f, 150.0f), COLOR_BLUE);

    // 3. The "Line" Case: 1-pixel wide/tall rectangles
    wm_draw_rect(&ctx, Rect_F32(50.0f, 50.0f, 300.0f, 1.0f), COLOR_WHITE);
    wm_draw_rect(&ctx, Rect_F32(50.0f, 50.0f, 1.0f, 300.0f), COLOR_WHITE);

    // 4. Out-of-Bounds: Partial Clipping
    // Top-left clipping
    wm_draw_rect(&ctx, Rect_F32(-50.0f, -50.0f, 100.0f, 100.0f), COLOR_RED);
    // Bottom-right clipping
    wm_draw_rect(&ctx, Rect_F32((F32)w - 50.0f, (F32)h - 50.0f, 100.0f, 100.0f), COLOR_GREEN);

    // 5. Fully Out-of-Bounds
    wm_draw_rect(&ctx, Rect_F32((F32)w + 100.0f, (F32)h + 100.0f, 50.0f, 50.0f), COLOR_MAGENTA);

    // 6. Negative Dimensions
    wm_draw_rect(&ctx, Rect_F32(500.0f, 500.0f, -100.0f, -50.0f), COLOR_YELLOW);
}


void wm_line_stress_test(WM_Context ctx) {
    const uint32_t LINE_COUNT = 16000000;
    uint32_t w = ctx.size.width;
    uint32_t h = ctx.size.height;

    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    printf("Starting Stress Test: 16 Million Lines (using stdlib rand)...\n");

    clock_t start_time = clock();

    for (uint32_t i = 0; i < LINE_COUNT; ++i) {
        Vec2F32 p1;
        // If your screen is > 32767 pixels, 
        // you'd need (rand() << 15 | rand()) to reach the edges.
        p1.x = (uint32_t)rand() % w;
        p1.y = (uint32_t)rand() % h;

        Vec2F32 p2;
        p2.x = (uint32_t)rand() % w;
        p2.y = (uint32_t)rand() % h;
        
        ColorRGBA color;
        // Packing random bits into the U32 color
        color.c = (uint32_t)rand() | 0xFF000000;

        wm_draw_line(&ctx, p1, p2, color);
    }

    clock_t end_time = clock();
    
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    if (time_spent > 0) {
        double mlps = (LINE_COUNT / time_spent) / 1000000.0;
        printf("Finished in %.2f seconds.\n", time_spent);
        printf("Performance: %.2f Million Lines Per Second (MLPS)\n", mlps);
    }
}

void x11_graphics(){

	Arena* arena = arena_alloc();

	WM_Context ctx = wm_open_window(arena, 
				 Rect_U16(100,100,1000,1000), 
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
					quit = true;
				}else if(event.type == ConfigureNotify){
					XConfigureEvent xce = event.xconfigure;
					ctx.size.width = (U16) xce.width;
					ctx.size.height = (U16) xce.height;
				}
			}	
			
			// Background
			wm_draw_rect(&ctx, Rect_F32(0, 0, ctx.size.width, ctx.size.height), RGBA(0,0,0,0));
		
			// 2d primitive testing -- test cases made by ai
			//line_draw_test(ctx);
			//triangle_draw_test(ctx);
			//circle_draw_test(ctx);	
			//rect_draw_test(ctx);
			wm_line_stress_test(ctx);
			wm_draw_window(&ctx);
			for(;;);	
		}
			
	}
	wm_close_window(&ctx);
	arena_release(arena);
}


void os_main(){
	
	


}


S32 entry_point(U64 argc, U8** argv){
	(void) argc;
	(void) argv;

	ScratchArenaScope(scratch, 0, 0){
		Str8 current_dir = os_get_current_path(scratch.arena);

		OS_Handle file_handle = os_file_open(Str8Lit("resources/customers-2000000.csv"), OS_AccessFlag_Read);
		OS_FileProperties props = os_properties_from_file_handle(file_handle);

		DateTime time = datetime_from_densetime(props.modified);
		time_t linux_time = os_lnx_time_from_datetime(time);
		DateTime universal_time = os_universal_time_from_local(time);
		DateTime local_time = os_local_time_from_universal(universal_time);
		
		str8_printf(stderr, "File name: %.*s\n", Str8VArg(props.name));
		str8_printf(stderr, "Bytes read: %lu\n", props.name.size);
		str8_printf(stderr, "Modified: %d %d %d %d %d\n", time.year, time.month_num , time.day, time.hour, time.min);
		str8_printf(stderr, "Linux Time: %lu\n", linux_time);
		str8_printf(stderr, "Universal Time: %d %d %d %d %d\n", universal_time.year, universal_time.month_num , universal_time.day, universal_time.hour, universal_time.min);
		str8_printf(stderr, "Local Time: %d %d %d %d %d\n", local_time.year, local_time.month_num , local_time.day, local_time.hour, local_time.min);

		os_file_close(file_handle);	
	}
	return 0;
}


int main(int argc, char** argv){os_entry_point(argc, (U8**) argv, &entry_point);}




