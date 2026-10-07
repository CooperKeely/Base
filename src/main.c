///////////////////////////////////////
/// cjk: header files include

#define OS_GFX_ENABLE
#define OS_LINUX_WAYLAND
#define RENDERER_VULKAN_ENABLE
#define FORMAT_OBJ_ENABLE

#include "base/base_inc.h"
#include "os/os_inc.h"

#include "base/base_inc.c"
#include "os/os_inc.c"


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
	
	Arena* arena = arena_alloc();

	// Open the window
	OS_GFX_Context* ctx = os_gfx_init_window(arena, 100, 100, 1000, 1000, Str8Lit("Software Renderer"));


	os_gfx_close_window(ctx);
	arena_release(arena);

	return 0;
}


int main(int argc, char** argv){os_entry_point(argc, (U8**) argv, &entry_point);}
