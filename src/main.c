///////////////////////////////////////
/// cjk: header files include

#define OS_GFX_ENABLE
#define RENDERER_SOFTWARE_ENABLE
#define FORMAT_OBJ_ENABLE

#include "base/base_inc.h"
#include "os/os_inc.h"
#include "format/format_inc.h"
#include "renderer/renderer_inc.h"

#include "base/base_inc.c"
#include "os/os_inc.c"
#include "format/format_inc.c"
#include "renderer/renderer_inc.c"


void line_draw_test(OS_GFX_WindowContext ctx){
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


void triangle_draw_test(OS_GFX_WindowContext ctx){
	
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

void circle_draw_test(OS_GFX_WindowContext ctx){
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

void rect_draw_test(OS_GFX_WindowContext ctx) {
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



static U32 xorshift_state = 42;

U32 fast_rand(void){
	xorshift_state ^= xorshift_state << 13;
	xorshift_state ^= xorshift_state >> 17;
	xorshift_state ^= xorshift_state << 5;
	return xorshift_state;
}


void sr_line_stress_test(OS_GFX_WindowContext* ctx) {
    const uint32_t LINE_COUNT = 16000000;
    uint32_t w = ctx->size.width;
    uint32_t h = ctx->size.height;

    printf("Starting Stress Test: 16 Million Lines (using stdlib rand)...\n");

    clock_t start_time = clock();

    for (uint32_t i = 0; i < LINE_COUNT; ++i) {
        Vec2F32 p1;
        // If your screen is > 32767 pixels, 
        // you'd need (rand() << 15 | rand()) to reach the edges.
        p1.x = (F32) (fast_rand() % w);
        p1.y = (F32) (fast_rand() % h);

        Vec2F32 p2;
        p2.x = (F32) (fast_rand() % w);
        p2.y = (F32) (fast_rand() % h);
        
        ColorRGBA color;
        // Packing random bits into the U32 color
        color.c = (U32)fast_rand() | 0xFF000000;

        wm_draw_line(ctx, p1, p2, color);
    }

    clock_t end_time = clock();
    
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    if (time_spent > 0) {
        double mlps = (LINE_COUNT / time_spent) / 1000000.0;
        printf("Finished in %.2f seconds.\n", time_spent);
        printf("Performance: %.2f Million Lines Per Second (MLPS)\n", mlps);
    }
}


// Assuming your types and functions are defined above or in a header
// typedef struct { char *str; U64 size; } Str8;

void test_f32(Str8 s, float expected) {
    float result = str8_to_f32(s);
    
    // Using a small epsilon for float comparison
    float epsilon = 0.00001f;
    if (fabsf(result - expected) < epsilon) {
        printf("[ PASS ] '%.*s' -> %f\n", Str8VArg(s), result);
    } else {
        printf("[ FAIL ] '%.*s' -> Expected %f, got %f\n", Str8VArg(s), expected, result);
    }
}

int test_str8_to_f32() {
    printf("[--- Starting str8_to_f32 Tests ---]\n");

    // Basic cases
    test_f32(Str8Lit("1"), 1.0f);
    test_f32(Str8Lit("123"), 123.0f);
    test_f32(Str8Lit("-50"), -50.0f);

    // Decimal cases
    test_f32(Str8Lit("1.5"), 1.5f);
    test_f32(Str8Lit("-1.5"), -1.5f);
    test_f32(Str8Lit("0.005"), 0.005f);
    test_f32(Str8Lit(".5"), 0.5f);   // Leading dot
    test_f32(Str8Lit("10."), 10.0f); // Trailing dot

    // Scientific notation (Common in high-poly OBJs)
    test_f32(Str8Lit("1e2"), 100.0f);
    test_f32(Str8Lit("1.5e3"), 1500.0f);
    test_f32(Str8Lit("100E-2"), 1.0f);
    test_f32(Str8Lit("-1.23e-2"), -0.0123f);

    // Edge cases
    test_f32(Str8Lit("-0.5"), -0.5f);
    test_f32(Str8Lit("0"), 0.0f);

    printf("[--- Tests Complete ---]\n");
    return 0;
}


void obj_parser_test(){
	Arena* arena = arena_alloc();
	FMT_OBJ_Object* obj_file = fmt_obj_object_init(arena, Str8Lit("./resources/cube.obj"));

	fmt_obj_parse_file(obj_file);

	Assert(obj_file->vertex_array.count == 8);
	Assert(obj_file->normal_array.count == 6);


	arena_release(arena);
}

void x11_graphics(){
	Arena* arena = arena_alloc();

	OS_GFX_WindowContext* ctx;
	ctx = os_gfx_open_window(arena, 
				Rect_U16(100,100,500,500), 
				Str8Lit("Demo"),
				0,
				RGBA(0,0,0,0),
				RGBA(0,0,0,0));

	sr_line_stress_test(ctx);
	os_gfx_draw_window(ctx);	
	os_sleep_milliseconds(200);
	os_gfx_close_window(ctx);
	arena_release(arena);
}



void os_file_props_test(){
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

}



S32 entry_point(U64 argc, U8** argv){
	(void) argc;
	(void) argv;
	
	//x11_graphics();
	//test_str8_to_f32();
	obj_parser_test();
	return 0;
}


int main(int argc, char** argv){os_entry_point(argc, (U8**) argv, &entry_point);}






