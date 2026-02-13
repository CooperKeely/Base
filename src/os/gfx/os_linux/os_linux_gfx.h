#ifndef OS_GFX_LINUX_H
#define OS_GFX_LINUX_H

// read only has namespace collisions with shm.h
# pragma push_macro("read_only")
# undef read_only

# include <xcb/xcb.h>
# include <xcb/shm.h>
# include <sys/ipc.h>
# include <sys/shm.h>

# pragma pop_macro("read_only")

struct OS_GFX_WindowContext{
	xcb_connection_t*	connection;
	xcb_screen_t*		screen;
	xcb_window_t		window;	
	xcb_setup_t* 		setup;
	xcb_gcontext_t		graphics_ctx;
	xcb_shm_seg_t		shared_mem_seg;
	U64			shared_mem_seg_id;
	RectU16			size;
	RectU16			over_size;
	Str8			name;
} ;


#endif //OS_GFX_LINUX_H
