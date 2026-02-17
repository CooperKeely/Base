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
	sr_draw_line(&ctx, Vec2_F32(50, 50), Vec2_F32(150, 50),  COLOR_WHITE); // Horizontal
	sr_draw_line(&ctx, Vec2_F32(50, 50), Vec2_F32(50, 150),  COLOR_WHITE); // Vertical

	// --- Group 2: 45 Degree Diagonals (Top Middle) ---
	sr_draw_line(&ctx, Vec2_F32(300, 50),  Vec2_F32(400, 150), COLOR_WHITE); // Down-Right
	sr_draw_line(&ctx, Vec2_F32(450, 150), Vec2_F32(550, 50),  COLOR_WHITE); // Up-Right
	sr_draw_line(&ctx, Vec2_F32(700, 150), Vec2_F32(600, 50),  COLOR_WHITE); // Up-Left
	sr_draw_line(&ctx, Vec2_F32(850, 50),  Vec2_F32(750, 150), COLOR_WHITE); // Down-Left

	// --- Group 3: Shallow Lines |dx| > |dy| (Center Row) ---
	// Rightward shallow
	sr_draw_line(&ctx, Vec2_F32(50, 300),  Vec2_F32(250, 350), COLOR_GREEN); // Octant 0
	sr_draw_line(&ctx, Vec2_F32(250, 370), Vec2_F32(50, 320),  COLOR_GREEN); // Octant 4 (Reverse)
	// Leftward shallow
	sr_draw_line(&ctx, Vec2_F32(550, 300), Vec2_F32(350, 350), COLOR_YELLOW); 
	sr_draw_line(&ctx, Vec2_F32(350, 370), Vec2_F32(550, 320), COLOR_YELLOW);

	// --- Group 4: Steep Lines |dy| > |dx| (Bottom Row) ---
	// Downward steep
	sr_draw_line(&ctx, Vec2_F32(50,  450), Vec2_F32(100, 650), COLOR_RED);   // Octant 1
	sr_draw_line(&ctx, Vec2_F32(120, 650), Vec2_F32(70,  450), COLOR_RED);   // Octant 5 (Reverse)
	// Upward steep
	sr_draw_line(&ctx, Vec2_F32(250, 650), Vec2_F32(300, 450), COLOR_BLUE);
	sr_draw_line(&ctx, Vec2_F32(320, 450), Vec2_F32(270, 650), COLOR_BLUE);

	// --- Group 5: Boundary/Clipping Test ---
	// These test if your loop handles coordinates outside [0, 1279]
	sr_draw_line(&ctx, Vec2_F32(1200, 10), Vec2_F32(1300, 100), COLOR_RED);  // Right edge clip
	sr_draw_line(&ctx, Vec2_F32(10, 700),  Vec2_F32(100, 800),  COLOR_BLUE); // Bottom edge clip
	sr_draw_line(&ctx, Vec2_F32(-50, 360), Vec2_F32(50, 360),   COLOR_GREEN); // Left edge clip
}


void triangle_draw_test(OS_GFX_WindowContext ctx){
	
	// 1. Degenerate Case: A Single Point
	// Should draw nothing or just one pixel.
	sr_draw_triangle(&ctx, Vec2_F32(100, 100), Vec2_F32(100, 100), Vec2_F32(100, 100), COLOR_RED); 

	// 2. Degenerate Case: A Flat Line
	// Three collinear points. Should look like a single horizontal line.
	sr_draw_triangle(&ctx, Vec2_F32(10, 10), Vec2_F32(50, 10), Vec2_F32(100, 10), COLOR_GREEN);

	// 3. Standard Case: Right-angled Triangle
	// Perfect for checking if the fill/lines align on axes.
	sr_draw_triangle(&ctx, Vec2_F32(200, 200), Vec2_F32(200, 300), Vec2_F32(300, 300), COLOR_BLUE);

	// 4. "The Needle": Very thin triangle
	// Tests the precision of your Bresenham/Rasterizer logic.
	sr_draw_triangle(&ctx, Vec2_F32(400, 100), Vec2_F32(405, 100), Vec2_F32(600, 500), COLOR_YELLOW);

	// 5. Clipping Case: Partially off-screen (Left/Top)
	// One or more vertices have coordinates near or at 0.
	sr_draw_triangle(&ctx, Vec2_F32(-10, -10), Vec2_F32(50, 10), Vec2_F32(10, 50), COLOR_MAGENTA);

	// 6. Clipping Case: Partially off-screen (Right/Bottom)
	// One or more vertices exceed ctx.size.width/height.
	U32 w = ctx.size.width;
	U32 h = ctx.size.height;
	sr_draw_triangle(&ctx, Vec2_F32(w-20, h-20), Vec2_F32(w+50, h-20), Vec2_F32(w-20, h+50), COLOR_CYAN);

	// 7. Winding Order Test
	// If your algorithm depends on clockwise/counter-clockwise, 
	// try drawing the same triangle with swapped vertices.
	sr_draw_triangle(&ctx, Vec2_F32(500, 500), Vec2_F32(550, 600), Vec2_F32(450, 600), COLOR_WHITE);
}

void circle_draw_test(OS_GFX_WindowContext ctx){
	U32 w = ctx.size.width;
	U32 h = ctx.size.height;
	Vec2F32 screen_center = Vec2_F32((F32)w / 2.0f, (F32)h / 2.0f );
	
	// 1. Large Radius: Mostly off-screen
	// Tests if your loop handles being 90% clipped without lagging.
	sr_draw_circle(&ctx, screen_center, (F32)w * 0.8f, COLOR_BLACK);

	// 2. Standard Case: Concentric Circles
	// This tests if the radius is being calculated consistently.
	// If they overlap or look "wobbly," your error term logic is off.
	sr_draw_circle(&ctx, screen_center, 150.0f, COLOR_BLUE);
	sr_draw_circle(&ctx, screen_center, 100.0f, COLOR_GREEN);
	sr_draw_circle(&ctx, screen_center, 50.0f,  COLOR_RED);

	// 3. The "Dot": Minimum Radius
	// Should draw a single pixel or a tiny 2x2/3x3 cluster.
	sr_draw_circle(&ctx, Vec2_F32(20.0f, 20.0f), 1.0f, COLOR_YELLOW);

	// 4. Edge Clipping: Center is off-screen
	// Only a "bow" or arc should be visible.
	sr_draw_circle(&ctx, Vec2_F32(0.0f, 0.0f), 50.0f, COLOR_MAGENTA);

	// 5. Side Clipping: Circle intersects the right/bottom boundaries
	// This is where many algorithms crash due to array out-of-bounds.
	sr_draw_circle(&ctx, Vec2_F32(w, h), 60.0f, COLOR_CYAN);

	// 6. Floating Point Precision: Tiny offset
	// Draw two circles with a 0.5 pixel difference. 
	// If they look identical, your algorithm is casting to int too early.
	sr_draw_circle(&ctx, Vec2_F32(150.0f, 150.0f), 30.0f, COLOR_WHITE);
	sr_draw_circle(&ctx, Vec2_F32(150.5f, 150.5f), 30.0f, COLOR_RED);	
}

void rect_draw_test(OS_GFX_WindowContext ctx) {
	U32 w = ctx.size.width;
	U32 h = ctx.size.height;

	// 1. Large Fill (Stress Test)
	sr_draw_rect(&ctx, Rect_F32(0.0f, 0.0f, (F32)w, (F32)h), COLOR_CYAN);

	// 2. Standard Case: A simple centered rectangle
	sr_draw_rect(&ctx, Rect_F32(100.0f, 100.0f, 200.0f, 150.0f), COLOR_BLUE);

	// 3. The "Line" Case: 1-pixel wide/tall rectangles
	sr_draw_rect(&ctx, Rect_F32(50.0f, 50.0f, 300.0f, 1.0f), COLOR_WHITE);
	sr_draw_rect(&ctx, Rect_F32(50.0f, 50.0f, 1.0f, 300.0f), COLOR_WHITE);

	// 4. Out-of-Bounds: Partial Clipping
	// Top-left clipping
	sr_draw_rect(&ctx, Rect_F32(-50.0f, -50.0f, 100.0f, 100.0f), COLOR_RED);
	// Bottom-right clipping
	sr_draw_rect(&ctx, Rect_F32((F32)w - 50.0f, (F32)h - 50.0f, 100.0f, 100.0f), COLOR_GREEN);

	// 5. Fully Out-of-Bounds
	sr_draw_rect(&ctx, Rect_F32((F32)w + 100.0f, (F32)h + 100.0f, 50.0f, 50.0f), COLOR_MAGENTA);

	// 6. Negative Dimensions
	sr_draw_rect(&ctx, Rect_F32(500.0f, 500.0f, -100.0f, -50.0f), COLOR_YELLOW);
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

        sr_draw_line(ctx, p1, p2, color);
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
void fmt_obj_print_line_array(FMT_OBJ_LineArray *array) {
    Assert(array);
    if (array->count == 0) return; // Safer than Assert if an array is legitimately empty

    // Use the first element to determine the category header
    FMT_OBJ_LineType type = array->array[0].line_type;
    const char *label = "Elements";

    switch(type) {
        case FMT_OBJ_LineType_Vertex:  label = "Vertices"; break;
        case FMT_OBJ_LineType_Normal:  label = "Normals";  break;
        case FMT_OBJ_LineType_Texture: label = "Textures"; break;
        case FMT_OBJ_LineType_Face:    label = "Faces";    break;
    }

    printf("--- %s (%llu) ---\n", label, array->count);

    for (U64 i = 0; i < array->count; i++) {
        FMT_OBJ_Line *line = &array->array[i];

        switch(line->line_type) {
            case FMT_OBJ_LineType_Vertex: {
                if (line->v.w == 1.0f) {
                    printf("v   %.4f %.4f %.4f\n", line->v.x, line->v.y, line->v.z);
                } else {
                    printf("v   %.4f %.4f %.4f %.4f\n", line->v.x, line->v.y, line->v.z, line->v.w);
                }
            } break;

            case FMT_OBJ_LineType_Normal: {
                printf("vn  %.4f %.4f %.4f\n", line->vn.x, line->vn.y, line->vn.z);
            } break;

            case FMT_OBJ_LineType_Texture: {
                printf("vt  %.4f %.4f\n", line->vt.x, line->vt.y);
            } break;

            case FMT_OBJ_LineType_Face: {
                printf("f   ");
                for (U32 f_idx = 0; f_idx < 3; f_idx++) {
                    U32 v  = line->face.corner[f_idx].v_idx;
                    U32 vt = line->face.corner[f_idx].vt_idx;
                    U32 vn = line->face.corner[f_idx].vn_idx;

                    // Standard OBJ formatting logic:
                    if (vt == 0 && vn == 0) {
                        printf("%u", v);           // Just v
                    } else if (vt != 0 && vn == 0) {
                        printf("%u/%u", v, vt);    // v/vt
                    } else if (vt == 0 && vn != 0) {
                        printf("%u//%u", v, vn);   // v//vn (Matches your cube.obj)
                    } else {
                        printf("%u/%u/%u", v, vt, vn); // v/vt/vn
                    }
                    
                    if (f_idx < 2) printf(" ");
                }
                printf("\n");
            } break;

            default: break;
        }
    }
}



void obj_parser_test(){
	Arena* arena = arena_alloc();
	FMT_OBJ_Object* obj_file = fmt_obj_object_init(arena, Str8Lit("./resources/cube.obj"));
	Assert(obj_file->vertex_array.capacity == 8);
	Assert(obj_file->normal_array.capacity == 6);
	Assert(obj_file->face_array.capacity == 12);

	fmt_obj_parse_file(obj_file);
	
	fmt_obj_print_line_array(&obj_file->vertex_array);
	fmt_obj_print_line_array(&obj_file->normal_array);
	//fmt_obj_print_line_array(&obj_file->texture_array);
	fmt_obj_print_line_array(&obj_file->face_array);

	arena_release(arena);
}

void draw_obj_test(){
	Arena* arena = arena_alloc();
	// Load the object
	FMT_OBJ_Object* obj_file = fmt_obj_object_init(arena, Str8Lit("./resources/diablo3_pose.obj"));
	fmt_obj_parse_file(obj_file);

	// Open the window
	OS_GFX_WindowContext* ctx;
	ctx = os_gfx_open_window(arena, 
				Rect_U16(100, 100, 1000, 1000), 
				Str8Lit("Software Renderer - Speed Test"),
				0,
				RGBA(0,0,0,0),
				RGBA(0,0,0,0));


	// 2. Setup Timing Logic
	U64 start_time = os_now_microseconds();
	U64 duration_us = 10 * Million(1); // 10 seconds in microseconds
	U64 frame_count = 0;

	// 3. The "As Fast As Possible" Loop
	while ((os_now_microseconds() - start_time) < duration_us) {
		
		sr_draw_rect(ctx, Rect_F32(0, 0, 1000, 1000), COLOR_BLACK);
		
		sr_draw_obj(ctx, obj_file, 0);
		os_gfx_draw_window(ctx);	
		
		frame_count++;
	}

	// 4. Calculate performance stats
	F64 total_seconds = (F64)(os_now_microseconds() - start_time) / F64Lit(1000000.0);
	str8_printf(stderr, "Rendered %lu frames in %.2f seconds (%.2f FPS)\n", 
				frame_count, total_seconds, (F64)frame_count / total_seconds);

	os_gfx_close_window(ctx);
	arena_release(arena);
}



void x11_graphics(){
	Arena* arena = arena_alloc();

	OS_GFX_WindowContext* ctx;
	ctx = os_gfx_open_window(arena, 
				Rect_U16(100,100,1000,1000), 
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
	//obj_parser_test();
	draw_obj_test();
	return 0;
}


int main(int argc, char** argv){os_entry_point(argc, (U8**) argv, &entry_point);}






