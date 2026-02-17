///////////////////////////////////////
/// cjk: Window API Functions 

OS_GFX_WindowContext* os_gfx_open_window(Arena* arena, RectU16 win_rect, Str8 window_name, ColorRGBA background_color){
	Assert(arena);	

	OS_GFX_WindowContext* result = ArenaPushStructZero(arena, OS_GFX_WindowContext);
	
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
	result->screen = xcb_setup_roots_iterator(result->setup).data;

	// create window
	result->window = xcb_generate_id(result->connection);
	value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	value_list[0] = result->screen->white_pixel;
	value_list[1] = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_STRUCTURE_NOTIFY;

	xcb_create_window(result->connection, 
		   XCB_COPY_FROM_PARENT,
		   result->window,
		   result->screen->root,
		   win_rect.x,
		   win_rect.y,
		   win_rect.width,
		   win_rect.height,
		   0,
		   XCB_WINDOW_CLASS_INPUT_OUTPUT,
		   result->screen->root_visual,
		   value_mask,
		   value_list
		);

	// create graphics context
	result->graphics_ctx = xcb_generate_id(result->connection);
	value_mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	value_list[0] = result->screen->black_pixel;
	value_list[1] = 0;
	xcb_create_gc(result->connection, result->graphics_ctx, result->window, value_mask, value_list);

	// set window name
	xcb_change_property(result->connection,
		     XCB_PROP_MODE_REPLACE,
		     result->window,
		     XCB_ATOM_WM_NAME,
		     XCB_ATOM_STRING, 
		     8,
		     window_name.size,
		     window_name.str);

	// map window to screen
	xcb_map_window(result->connection, result->window);
	xcb_flush(result->connection);

	// test if shm is enabled
	xcb_shm_query_version_reply_t* version = xcb_shm_query_version_reply(result->connection, xcb_shm_query_version(result->connection), NULL);
	free(version);

	// create shared memory region
	U64 total_size = win_rect.width * win_rect.height * sizeof(ColorBGRA);  
	
	result->shm_info.shmid = shmget(IPC_PRIVATE, total_size, IPC_CREAT|0600);
	result->shm_info.shmaddr = shmat(result->shm_info.shmid, 0, 0);
	result->shm_info.shmseg = xcb_generate_id(result->connection);

	xcb_shm_attach(result->connection, result->shm_info.shmseg, result->shm_info.shmid, 0);
	shmctl(result->shm_info.shmid, IPC_RMID, 0);

	result->image = xcb_image_create_native(result->connection,
			 win_rect.width, 
			 win_rect.height,
			 XCB_IMAGE_FORMAT_Z_PIXMAP,
			 result->screen->root_depth,
			 result->shm_info.shmaddr,
			 total_size,
			 0
		  );
	
	// wait for the window to be mapped before giving it to the user
	xcb_generic_event_t *ev;
	while ((ev = xcb_wait_for_event(result->connection))) {
	    if ((ev->response_type & ~0x80) == XCB_MAP_NOTIFY) {
		free(ev);
		break;
	    }
	    free(ev);
	}
	
	return result;	
}

void os_gfx_resize_window(OS_GFX_WindowContext* ctx, U16 width, U16 height){
	NotImplemented;
}

void os_gfx_move_window(OS_GFX_WindowContext* ctx, U16 x, U16 y){
	NotImplemented;
}


void os_gfx_resize_and_move_window(OS_GFX_WindowContext* ctx, RectU16 new_size){
	NotImplemented;
}


void os_gfx_draw_window(OS_GFX_WindowContext* ctx){

	// put shm image to window
	xcb_shm_put_image(
		ctx->connection,
		ctx->window,
		ctx->graphics_ctx,
		ctx->image->width,
		ctx->image->height,
		0,
		0,
		ctx->image->width,
		ctx->image->height,
		0,
		0,
		ctx->image->depth,
		ctx->image->format,
		false,
		ctx->shm_info.shmseg,
		0
	);	

	// flush window
	xcb_flush(ctx->connection);


}

void os_gfx_close_window(OS_GFX_WindowContext* ctx){
	xcb_shm_detach(ctx->connection, ctx->shm_info.shmseg);
	shmdt(ctx->shm_info.shmaddr);
	
	// NOTE: set the base pointer to null because it has already been freed by shm
	// so it dosent get double freed by xcb_image_destroy()
	ctx->image->base = NULL;

        xcb_image_destroy(ctx->image);
	xcb_destroy_window(ctx->connection, ctx->window);
	xcb_disconnect(ctx->connection);
}

void os_gfx_register_input_events(OS_GFX_WindowContext* ctx, OS_GFX_EventFlag flags){
	
	
}

U32 os_gfx_num_of_pending_events(OS_GFX_WindowContext* ctx){
	NotImplemented;
}
