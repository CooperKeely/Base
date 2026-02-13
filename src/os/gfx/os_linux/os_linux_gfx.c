///////////////////////////////////////
/// cjk: Window API Functions 

OS_GFX_WindowContext* os_gfx_open_window(Arena* arena, RectU16 win_rect, Str8 window_name, U16 border_width, ColorRGBA border_color,  ColorRGBA background_color){
	Assert(arena);	

	OS_GFX_WindowContext* result = ArenaPushStruct(arena, OS_GFX_WindowContext);
	
	result->size = win_rect;
	result->name = window_name;
	U32 value_mask = 0;
	U32 value_list[2];


	// get connection
	result->connection = xcb_connect(NULL, NULL);
	if(xcb_connection_has_error(result->connection)){
		InvalidPath;
	}

	// get screen
	result->setup = xcb_get_setup(result->connection);
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(result->setup);
	result->screen = iter.data;

	// create graphics context
	result->graphics_ctx = xcb_generate_id(result->connection);
	result->window = result->screen->root;
	
	value_mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	value_list[0] = result->screen->black_pixel;
	value_list[1] = 0;
	xcb_create_gc(result->connection, result->graphics_ctx, result->window, value_mask, value_list);

	// create window
	result->window = xcb_generate_id(result->connection);
	value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	value_list[0] = result->screen->white_pixel;
	value_list[1] = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_STRUCTURE_NOTIFY

	xcb_create_window(result->connection, 
		   XCB_COPY_FROM_PARENT,
		   result->window,
		   result->screen->root,
		   win_rect.x,
		   win_rect.y,
		   win_rect.width,
		   win_rect.height,
		   border_width,
		   XCB_WINDOW_CLASS_INPUT_OUTPUT,
		   result->screen->root_visual,
		   value_mask,
		   value_list);

	// set window name
	xcb_change_property(result->connection,
		     XCB_PROP_MODE_REPLACE,
		     result->window,
		     XCB_ATOM_WM_NAME,
		     XCB_ATOM_STRING, 
		     8,
		     window_name.size,
		     window_name.str);


	Assert(result->connection);
	Assert(result->screen);

	result->image = XShmCreateImage(result->display,
				DefaultVisualOfScreen(result->screen),
				DefaultDepthOfScreen(result->screen),
				ZPixmap,
				NULL,
				&result->shm_info,
				result->over_size.width, result->over_size.height);

	Assert(result->image);

	// create shared memory region
	U64 total_size = result->image->bytes_per_line * result->image->height;
	result->shared_mem_seg = xcb_generate_id(result->connection);
	result->shared_mem_id = shmget(IPC_PRIVATE, total_size, IPC_CREAT|0600);

	result->shm_info.shmaddr = result->image->data = shmat(result->shm_info.shmid, 0, 0);
	result->shm_info.readOnly = False;

	Status status = XShmAttach(result->display, &result->shm_info);
	XSync(result->display, False);
	shmctl(result->shm_info.shmid, IPC_RMID, 0);

	Assert(status);

	xcb_map_window(result->connection, result->window);
	return result;	
}

void os_gfx_resize_window(OS_GFX_WindowContext* ctx, U16 width, U16 height){
	XResizeWindow(ctx->display, ctx->window, width, height);
}

void os_gfx_move_window(OS_GFX_WindowContext* ctx, U16 x, U16 y){
	XMoveWindow(ctx->display, ctx->window, x, y);
}


void os_gfx_resize_and_move_window(OS_GFX_WindowContext* ctx, RectU16 new_size){
	U32 change_mask = WM_WindowCfgUpdate_X | WM_WindowCfgUpdate_Y | WM_WindowCfgUpdate_Width | WM_WindowCfgUpdate_Height;
	XWindowChanges changes = {
		.x = new_size.x,
		.y = new_size.y,
		.width = new_size.width,
		.height = new_size.height
	};
	XConfigureWindow(ctx->display, ctx->window, change_mask, &changes);
}


void os_gfx_draw_window(OS_GFX_WindowContext* ctx){
	Assert(ctx);
	Assert(ctx->display);
	Assert(ctx->window);
	Assert(ctx->image);

#if HAS_SYS_SHM	
	XShmPutImage(ctx->display, ctx->window, ctx->graphics_ctx, ctx->image, 0, 0, 0, 0, ctx->size.width, ctx->size.height, False);
	XSync(ctx->display, False);
#else
	XPutImage(ctx->display, ctx->window, ctx->graphics_ctx, ctx->image, 0, 0, 0, 0, ctx->size.width, ctx->size.height);
	XFlush(ctx->display);
#endif
}

void gfx_close_window(OS_GFX_WindowContext* ctx){
	XUnmapWindow(ctx->display, ctx->window);
	XDestroyWindow(ctx->display, ctx->window);
	XFreeGC(ctx->display, ctx->graphics_ctx);
	XCloseDisplay(ctx->display);
}

void os_gfx_register_input_events(OS_GFX_WindowContext* ctx, WM_EventFlag flags){
	long event_mask = 0;	

	for EachIndex(i, ArrayCount(wm_event_mask_map)){
		if(flags & wm_event_mask_map[i].flag){
			event_mask |= wm_event_mask_map[i].event_mask;
		}
	}
	
	XSelectInput(ctx->display, ctx->window, event_mask);
}

U32 os_gfx_num_of_pending_events(OS_GFX_WindowContext* ctx){
	return XPending(ctx->display);
}
