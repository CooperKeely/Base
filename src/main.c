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


void draw_obj_test(){
	Arena* arena = arena_alloc();
	
	// Load the object
	FMT_OBJ_Object* obj_file = fmt_obj_object_init(arena, Str8Lit("./resources/african_head.obj"));
	fmt_obj_parse_file(obj_file);

	// Open the window
	OS_GFX_Context* ctx = os_gfx_init_window(arena, 100, 100, 1000, 1000, Str8Lit("Software Renderer - Speed Test"));

	os_gfx_set_target_fps(60);

	while (!os_gfx_window_should_close()) {
		os_gfx_begin_drawing();

		os_gfx_clear_background(COLOR_BLACK);		

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
