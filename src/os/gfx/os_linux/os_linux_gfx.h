#ifndef OS_GFX_LINUX_H
#define OS_GFX_LINUX_H

// read only has namespace collisions with shm.h
# pragma push_macro("read_only")
# undef read_only

# include <xcb/xcb.h>
# include <xcb/xproto.h>
# include <xcb/xcb_icccm.h>
# include <xcb/xcb_pixel.h>
# include <xcb/xcb_image.h>
# include <xcb/shm.h>

# include <sys/ipc.h>
# include <sys/shm.h>

# pragma pop_macro("read_only")

typedef struct{
	xcb_connection_t*		connection;
	xcb_screen_t*			screen;
	xcb_window_t			window;	
	const xcb_setup_t* 		setup;
	xcb_gcontext_t			graphics_ctx;
	xcb_atom_t			close_window_atom;

	xcb_image_t*			frame_buffer[2];
	xcb_shm_segment_info_t		shm_info[2];
	B32				current_frame_buffer;
} OS_LNX_GFX_Context;

global OS_LNX_GFX_Context* glb_os_gfx_linux_context;

void os_lnx_gfx_set_current_global_context(OS_GFX_LinuxContext* ctx);
OS_LNX_GFX_Context* os_lnx_gfx_get_current_global_context();

#endif //OS_GFX_LINUX_H
