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

	// window setup 
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
			
	// create a window
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

	// create a xcb graphics context
	lnx_ctx->graphics_ctx = xcb_generate_id(lnx_ctx->connection);
	value_mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	value_list[0] = lnx_ctx->screen->black_pixel;
	value_list[1] = 0;

	xcb_create_gc(lnx_ctx->connection,
	       lnx_ctx->graphics_ctx,
	       lnx_ctx->window,
	       value_mask,
	       value_list);


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

	// map the window
	xcb_map_window(lnx_ctx->connection, lnx_ctx->window);
	xcb_flush(lnx_ctx->connection);
	
	// hang unil the window is ready
	while(!ctx->window.is_ready) os_gfx_poll_input_events();

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

void os_gfx_swap_screen_buffer(void){
	OS_GFX_LinuxContext* lnx_ctx = &glb_os_gfx_linux_context;
	U32 frame = lnx_ctx->current_frame_buffer;
	xcb_image_t* image = lnx_ctx->frame_buffer[frame];
	
	// put shm image to window
	xcb_shm_put_image(lnx_ctx->connection,
			lnx_ctx->window,
			lnx_ctx->graphics_ctx,
			image->width,
			image->height,
			0, 0,
			image->width,
			image->height,
			0, 0,
			image->depth,
			image->format,
			BASE_TRUE,
			lnx_ctx->shm_info[frame].shmseg,
			0);	

	// flush window
	xcb_flush(lnx_ctx->connection);
	
	// XOR the current frame buffer state
	lnx_ctx->current_frame_buffer ^= 1;

}

void os_gfx_set_window_size(U32 w, U32 h){
	// if window not resizeable don't do so
	if(!os_gfx_flag_is_window_state(OS_GFX_WindowConfigFlag_Resizeable)) continue;

	U32 width = ctx->window.screen_size.x;
	U32 height = ctx->window.screen_size.y;

	U32 min_width = ctx->window.screen_min_size.x;
	U32 max_width = ctx->window.screen_max_size.x;

	U32 min_height = ctx->window.screen_min_size.y;
	U32 max_height = ctx->window.screen_max_size.y;

	if(w != width || h != height){
		U32 new_width;
		U32 new_height;
		if(max_width == 0 || max_height == 0){
			new_width = event->width;
			new_widht = event->height;
		}else{
			new_width = ClampBot(ClampTop(event->width, min_width), max_width);
			new_height = ClampBot(ClampTop(event->height, min_height), max_height);
		}

		U32 mask = XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
		U32 values[] = {
			new_width,
			new_height
		};

		// send a configuration request
		xcb_configure_window(lnx_ctx->connection,
		       		lnx_ctx->window,
		       		mask,
		       		values);

		
	}
}

void os_gfx_poll_input_events(void){
	OS_GFX_LinuxContext* lnx_ctx = &glb_os_gfx_linux_context;
	OS_GFX_Context* ctx = &glb_os_gfx_context;
	
	xcb_generic_event_t* e;
	while((e= xcb_poll_for_event(lnx_ctx->connection))){
		switch(e->reponse_type & ~0x80){
			case XCB_EXPOSE:{
				xcb_expose_event_t* event = (xcb_expose_event_t*)e;
			}break;
			case XCB_KEY_PRESS:{
				xcb_key_press_event_t* event = (xcb_key_press_event_t*)e;
			}break;
			case XCB_KEY_RELEASE:{
				xcb_key_release_event_t* event = (xcb_key_release_event_t*)e;
			}break;
			case XCB_BUTTON_PRESS:{
				xcb_button_press_event_t* event = (xcb_button_press_event_t*)e;
			}break;
			case XCB_BUTTON_RELEASE:{
				xcb_button_release_event_t* event = (xcb_button_release_event_t*)e;
			}break;
			case XCB_MOTION_NOTIFY:{
				xcb_motion_notify_event_t* event = (xcb_motion_notify_event_t*)e;
			}break;
			case XCB_ENTER_NOTIFY:{
				xcb_enter_notify_event_t* event = (xcb_enter_notify_event_t*)e;
			}break;
			case XCB_LEAVE_NOTIFY:{
				xcb_leave_notify_event_t* event = (xcb_leave_notify_event_t*)e;
			}break;
			case XCB_FOCUS_IN:{
				xcb_focus_in_event_t* event = (xcb_focus_in_event_t*)e;
			}break;
			case XCB_FOCUS_OUT:{
				xcb_focus_out_event_t* event = (xcb_focus_out_event_t*)e;
			}break;
			case XCB_KEYMAP_NOTIFY:{
				xcb_keymap_notify_event_t* event = (xcb_keymap_notify_event_t*)e;
			}break;
			case XCB_GRAPHICS_EXPOSURE:{
				xcb_graphics_exposure_event_t* event = (xcb_graphics_exposure_event_t*)e;
			}break;
			case XCB_NO_EXPOSURE:{
				xcb_no_exposure_event_t* event = (xcb_no_exposure_event_t*)e;
			}break;
			case XCB_VISIBILITY_NOTIFY:{
				xcb_visibility_notify_event_t* event = (xcb_visibility_notify_event_t*)e;
			}break;
			case XCB_CREATE_NOTIFY:{
				xcb_create_notify_event_t* event = (xcb_create_notify_event_t*)e;
			}break;
			case XCB_DESTROY_NOTIFY:{
				xcb_destroy_notify_event_t* event = (xcb_destroy_notify_event_t*)e;
			}break;
			case XCB_UNMAP_NOTIFY:{
				xcb_unmap_notify_event_t* event = (xcb_unmap_notify_event_t*)e;
			}break;
			case XCB_MAP_NOTIFY:{
				xcb_map_notify_event_t* event = (xcb_map_notify_event_t*)e;
			}break;
			case XCB_MAP_REQUEST:{
				xcb_map_request_event_t* event = (xcb_map_request_event_t*)e;
			}break;
			case XCB_REPARENT_NOTIFY:{
				xcb_reparent_notify_event_t* event = (xcb_reparent_notify_event_t*)e;
			}break;
			case XCB_CONFIGURE_NOTIFY:{
				xcb_configure_notify_event_t* event = (xcb_configure_notify_event_t*)e;
				if(event->width != ctx->window.screen_size.x || event->height != ctx->window.screensize.y){
					ctx->window.screen_size.x = event->width;
					ctx->window.screen_size.y = event->height;
					ctx->window.resized_last_frame = BASE_TRUE;	
					os_gfx_reset_frame_buffers();
				}else if( event->x != ctx->window.position.x || event->y != ctx->window.position.y){
					ctx->window.previous_position.x = ctx->window.position.x;
					ctx->window.previous_position.y = ctx->window.position.y;
					ctx->window.position.x = event->x;
					ctx->window.position.y = event->y;

				}else{
					// TODO: add logging system
				}
			}break;
			case XCB_CONFIGURE_REQUEST:{
				xcb_configure_request_event_t* event = (xcb_configure_request_event_t*)e;
			}break;
			case XCB_GRAVITY_NOTIFY:{
				xcb_gravity_notify_event_t* event = (xcb_gravity_notify_event_t*)e;
			}break;
			case XCB_RESIZE_REQUEST:{
				xcb_resize_request_event_t* event = (xcb_resize_request_event_t*)e;
				os_gfx_set_window_size(event->width, event->height);
			}break;
			case XCB_CIRCULATE_NOTIFY:{
				xcb_circulate_notify_event_t* event = (xcb_circulate_notify_event_t*)e;
			}break;
			case XCB_CIRCULATE_REQUEST:{
				xcb_circulate_request_event_t* event = (xcb_circulate_request_event_t*)e;
			}break;
			case XCB_PROPERTY_NOTIFY:{
				xcb_property_notify_event_t* event = (xcb_property_notify_event_t*)e;
			}break;
			case XCB_SELECTION_CLEAR:{
				xcb_selection_clear_event_t* event = (xcb_selection_clear_event_t*)e;
			}break;
			case XCB_SELECTION_REQUEST:{
				xcb_selection_request_event_t* event = (xcb_selection_request_event_t*)e;
			}break;
			case XCB_SELECTION_NOTIFY:{
				xcb_selection_notify_event_t* event = (xcb_selection_notify_event_t*)e;
			}break;
			case XCB_COLORMAP_NOTIFY:{
				xcb_colormap_notify_event_t* event = (xcb_colormap_notify_event_t*)e;
			}break;
			case XCB_CLIENT_MESSAGE:{
				xcb_client_message_event_t* event = (xcb_client_message_event_t*)e;
				if(event->data.data32[0] == lnx_ctx->close_window_atom){
					ctx->window.should_close = BASE_TRUE;
				}
			}break;
			case XCB_MAPPING_NOTIFY:{
				xcb_mapping_notify_event_t* event = (xcb_mapping_notify_event_t*)e;
			}break;
			case XCB_GE_GENERIC:{
				xcb_ge_generic_event_t* event = (xcb_ge_generic_event_t*)e;
			}break;
			default:{

			}
		}
	}

	free(e);
}

