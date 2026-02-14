#ifndef OS_GFX_LINUX_H
#define OS_GFX_LINUX_H

// read only has namespace collisions with shm.h
# pragma push_macro("read_only")
# undef read_only

# include <xcb/xcb.h>
# include <xcb/xproto.h>
# include <xcb/xcb_pixel.h>
# include <xcb/xcb_image.h>
# include <xcb/shm.h>


# include <sys/ipc.h>
# include <sys/shm.h>

# pragma pop_macro("read_only")

struct OS_GFX_WindowContext{
	xcb_connection_t*		connection;
	xcb_screen_t*			screen;
	xcb_window_t			window;	
	xcb_image_t*			image;
	const xcb_setup_t* 		setup;
	xcb_gcontext_t			graphics_ctx;
	xcb_shm_segment_info_t		shm_info;
	RectU16				size;
	RectU16				over_size;
	Str8				name;
} ;


#endif //OS_GFX_LINUX_H
