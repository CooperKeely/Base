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



static U32 xorshift_state = 42;

U32 fast_rand(void){
	xorshift_state ^= xorshift_state << 13;
	xorshift_state ^= xorshift_state >> 17;
	xorshift_state ^= xorshift_state << 5;
	return xorshift_state;
}

void sr_line_stress_test() {
	const uint32_t LINE_COUNT = 16000000;
	OS_GFX_Context* ctx = &glb_os_gfx_context;
	uint32_t w = os_gfx_get_screen_width();
	uint32_t h = os_gfx_get_screen_height();

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

		sr_draw_line(p1, p2, color);
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
	FMT_OBJ_Object* obj_file = fmt_obj_object_init(arena, Str8Lit("./resources/african_head.obj"));
	fmt_obj_parse_file(obj_file);

	// Open the window
	os_gfx_init_window(100, 100, 1000, 1000, Str8Lit("Software Renderer - Speed Test"));

		
	while (!os_gfx_window_should_close()) {
		os_gfx_begin_drawing();

		sr_draw_rect(Rect_F32(0, 0, 1000, 1000), COLOR_BLACK);
		
		sr_draw_obj(obj_file, 0);

		os_gfx_end_drawing();
	}

	os_gfx_close_window();
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






