///////////////////////////////////////
/// cjk: header files include


#include "base/base_inc.h"
#include "wm/wm_inc.h"
#include "base/base_inc.c"



S32 entry_point(U64 argc, U8** argv){
	(void) argc;
	(void) argv;
	
	Arena* arena = arena_alloc();

	// Open the window
	// give the reference to the context when the user does
	// wm_init();
	// then on creation of the window we can pass the WM_Backend handle
	// so they can easily just pass that to the rendering backend
	WM_Context* ctx = wm_init_window(100, 100, 1000, 1000, Str8Lit("Software Renderer"));

	WM_BackendHandle handle = {0};
	if(!wm_get_backend_handle(ctx, &handle)){ InvalidPath; }
	
	// Initialize Rendering Context

	wm_close_window(ctx);
	arena_release(arena);

	return 0;
}


int main(int argc, char** argv){os_entry_point(argc, (U8**) argv, &entry_point);}
