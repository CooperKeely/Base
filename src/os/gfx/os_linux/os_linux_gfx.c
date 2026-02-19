///////////////////////////////////////
/// cjk: Window API Functions 

void os_gfx_init_platform(void){
	MemoryZeroStruct(&glb_os_gfx_linux_context);

	OS_GFX_LinuxContext* lnx_ctx = &glb_os_gfx_linux_context;
	OS_GFX_Context* ctx = &glb_os_gfx_context;

	U32 value_mask = 0;
	U32 value_list[2];

	// get connection
	lnx_ctx->connection = xcb_connect(NULL, NULL);
	if(xcb_connection_has_error(lnx_ctx->connection)){
		InvalidPath;
	}

	// register standard XCB cookies 
	xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom(lnx_ctx->connection, 1, 12, "WM_PROTOCOLS");
	xcb_intern_atom_cookie_t delete_cookie = xcb_intern_atom(lnx_ctx->connection, 0, 16, "WM_CLOSE_WINDOW");

	// get screen
	lnx_ctx->setup = xcb_get_setup(lnx_ctx->connection);
	lnx_ctx->screen = xcb_setup_roots_iterator(lnx_ctx->setup).data;

	// create window
	lnx_ctx->window = xcb_generate_id(lnx_ctx->connection);

	value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK ;
	value_list[0] = lnx_ctx->screen->white_pixel;
	value_list[1] = XCB_EVENT_MASK_KEY_PRESS 	| 	XCB_EVENT_MASK_KEY_RELEASE	|
			XCB_EVENT_MASK_BUTTON_PRESS 	|	XCB_EVENT_MASK_BUTTON_RELEASE	|
			XCB_EVENT_MASK_ENTER_WINDOW	|	XCB_EVENT_MASK_LEAVE_WINDOW	|
			XCB_EVENT_MASK_POINTER_MOTION 	|	XCB_EVENT_MASK_EXPOSURE		|
			XCB_EVENT_MASK_STRUCTURE_NOTIFY |	XCB_EVENT_MASK_FOCUS_CHANGE	|
			XCB_EVENT_MASK_PROPERTY_CHANGE	|	XCB_EVENT_MASK_VISIBILITY_CHANGE|
			XCB_EVENT_MASK_BUTTON_MOTION	|	XCB_EVENT_MASK_BUTTON_1_MOTION	|	
			XCB_EVENT_MASK_BUTTON_2_MOTION	|	XCB_EVENT_MASK_BUTTON_3_MOTION	|	
			XCB_EVENT_MASK_BUTTON_4_MOTION	|	XCB_EVENT_MASK_BUTTON_5_MOTION	|	
			XCB_EVENT_MASK_KEYMAP_STATE	|	XCB_EVENT_MASK_RESIZE_REDIRECT	;
			

	xcb_create_window(lnx_ctx->connection, 
			XCB_COPY_FROM_PARENT,
			lnx_ctx->window,
			lnx_ctx->screen->root,
			ctx->window.position.x,
			ctx->window.position.y,
			ctx->window.screen_size.x,
			ctx->window.screen_size.y,
			0,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			lnx_ctx->screen->root_visual,
			value_mask,
			value_list);
		
	// set window name
	xcb_change_property(lnx_ctx->connection,
			XCB_PROP_MODE_REPLACE,
			lnx_ctx->window,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING, 
			8,
			ctx->window.title.size,
			ctx->window.title.str);
	
	// collect replies 
	xcb_intern_atom_reply_t* protocols_reply = xcb_intern_atom_reply(lnx_ctx->connection, protocols_cookie, NULL);
	xcb_intern_atom_reply_t* delete_reply = xcb_intern_atom_reply(lnx_ctx->connection, delete_cookie, NULL);
	
	// tell the window mangager to send message instad of killing us 
	xcb_change_property(lnx_ctx->connection,
			XCB_PROP_MODE_REPLACE,
			lnx_ctx->window,
			protocols_reply->atom,
			XCB_ATOM_ATOM, 
			32,
		     	1,
			&delete_reply->atom);
	
	// save the atom to check for it later in event loop
	lnx_ctx->close_window_atom = delete_reply->atom;	
	
	free(protocols_reply);
	free(delete_reply);	

	os_gfx_reset_frame_buffers();
}

void os_gfx_close_platform(void){
	OS_GFX_LinuxContext* lnx_ctx = &glb_os_gfx_linux_context;

	for EachIndex(idx, 2){
		xcb_shm_detach(lnx_ctx->connection, lnx_ctx->shm_info[idx].shmseg);
		shmdt(lnx_ctx->shm_info[idx].shmaddr);
	
		// NOTE: set the base pointer to null because it has already been freed by shm
		// so it dosent get double freed by xcb_image_destroy()

		lnx_ctx->frame_buffer[idx]->base = NULL;
        	xcb_image_destroy(lnx_ctx->frame_buffer[idx]);
		lnx_ctx->frame_buffer[idx] = NULL;
	}
	
	if(lnx_ctx->window) xcb_destroy_window(lnx_ctx->connection, lnx_ctx->window);
	if(lnx_ctx->connection) xcb_disconnect(lnx_ctx->connection);
}

void os_gfx_reset_frame_buffers(void){
	OS_GFX_LinuxContext* lnx_ctx = &glb_os_gfx_linux_context;
	OS_GFX_Context* ctx = &glb_os_gfx_context;
	lnx_ctx->current_frame_buffer = 0;

	for EachIndex(idx, 2){
		// free old shared frame buffer memory
		if(lnx_ctx->frame_buffer[idx]){
			xcb_shm_detach(lnx_ctx->connection, lnx_ctx->shm_info[idx].shmseg);
			free(lnx_ctx->frame_buffer[idx]);
			shmdt(lnx_ctx->shm_info[idx].shmaddr);
		}
		
		U64 total_size = ctx->window.screen_size.x * ctx->window.screen_size.y * sizeof(ColorBGRA);

		
		// get the shared memory
		lnx_ctx->shm_info[idx].shmid = shm_get(IPC_PRIVATE, total_size, IPC_CREAT|0777);
		lnx_ctx->shm_info[idx].shmaddr = (void*) shmat(lnx_ctx->shm_info[idx], 0, 0);
		Assert(lnx_ctx->shm_info[idx].shmaddr != (void*)-1);
		
		// create a new image
		lnx_ctx->frame_buffer[idx] = xcb_create_image_native(
			lnx_ctx->connection,
			ctx->window.screen_size.x,
			ctx->window.screen_size.y,
			XCB_IMAGE_FORMAT_Z_PIXMAP,
			lnx_ctx->screen->root_depth,
			lnx_ctx->shm_info[idx].shmaddr,
			total_size,
			lnx_ctx->shm_info[idx].shmaddr	
		);

		lnx_ctx->shm_info[idx].shmseg = xcb_generate_id(lnx_ctx->connection);

		xcb_shm_attach(lnx_ctx->connection, 
		 		lnx_ctx->shm_info[idx].shmseg, 
		 		lnx_ctx->shm_info[idx].shmid, 
		 		0);

		xcb_flush(lnx_ctx->connection);

		shm_ctl(lnx_ctx->shm_info[idx].shmid, IPC_RMID, 0);	

	}

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
		BASE_FALSE,
		ctx->shm_info.shmseg,
		0
	);	

	// flush window
	xcb_flush(ctx->connection);


}

