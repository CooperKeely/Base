///////////////////////////////////////
/// cjk: Window API Functions 

void os_gfx_init_platform(Arena* arena){
	OS_LNX_GFX_Context* lnx_ctx = ArenaPushStructZero(arena, OS_LNX_GFX_Context);
	OS_GFX_Context* ctx = os_gfx_get_current_global_context();

	os_lnx_gfx_set_current_global_context(lnx_ctx);

	U32 value_mask = 0;
	U32 value_list[2];

	// get connection
	lnx_ctx->connection = xcb_connect(NULL, NULL);
	if(xcb_connection_has_error(lnx_ctx->connection)){
		InvalidPath;
	}

	// register standard XCB cookies 
	xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom(lnx_ctx->connection, 0, 12, "WM_PROTOCOLS");
	xcb_intern_atom_cookie_t delete_cookie = xcb_intern_atom(lnx_ctx->connection, 0, 16, "WM_DELETE_WINDOW");

	// get screen
	lnx_ctx->setup = xcb_get_setup(lnx_ctx->connection);
	lnx_ctx->screen = xcb_setup_roots_iterator(lnx_ctx->setup).data;

	// window setup 
	lnx_ctx->window = xcb_generate_id(lnx_ctx->connection);

	value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK ;
	value_list[0] = XCB_NONE;
	value_list[1] = XCB_EVENT_MASK_KEY_PRESS 	| 	XCB_EVENT_MASK_KEY_RELEASE	|
			XCB_EVENT_MASK_BUTTON_PRESS 	|	XCB_EVENT_MASK_BUTTON_RELEASE	|
			XCB_EVENT_MASK_ENTER_WINDOW	|	XCB_EVENT_MASK_LEAVE_WINDOW	|
			XCB_EVENT_MASK_POINTER_MOTION 	|	XCB_EVENT_MASK_EXPOSURE		|
			XCB_EVENT_MASK_STRUCTURE_NOTIFY |	XCB_EVENT_MASK_FOCUS_CHANGE	|
			XCB_EVENT_MASK_PROPERTY_CHANGE	|	XCB_EVENT_MASK_VISIBILITY_CHANGE|
			XCB_EVENT_MASK_BUTTON_MOTION	|	XCB_EVENT_MASK_BUTTON_1_MOTION	|	
			XCB_EVENT_MASK_BUTTON_2_MOTION	|	XCB_EVENT_MASK_BUTTON_3_MOTION	|	
			XCB_EVENT_MASK_BUTTON_4_MOTION	|	XCB_EVENT_MASK_BUTTON_5_MOTION	|	
			XCB_EVENT_MASK_KEYMAP_STATE	;
			
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
	
	// get display size for allocation
	ctx->window.display_size.x = lnx_ctx->screen->width_in_pixels;
	ctx->window.display_size.y = lnx_ctx->screen->height_in_pixels;

	os_gfx_set_window_max_size(ctx->window.display_size.x, ctx->window.display_size.y);
	os_gfx_set_window_min_size(0, 0);

	// collect replies 
	xcb_intern_atom_reply_t* protocols_reply = xcb_intern_atom_reply(lnx_ctx->connection, protocols_cookie, NULL);
	xcb_intern_atom_reply_t* delete_reply = xcb_intern_atom_reply(lnx_ctx->connection, delete_cookie, NULL);
	Assert(protocols_reply && delete_reply);	
	
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
	
	while(!os_gfx_is_window_ready()) os_gfx_poll_input_events();

}

void os_gfx_close_platform(void){
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();

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

void os_lnx_gfx_set_current_global_context(OS_LNX_GFX_Context* ctx){
	glb_os_lnx_gfx_context = ctx;
}

OS_LNX_GFX_Context* os_lnx_gfx_get_current_global_context(){
	return glb_os_lnx_gfx_context;
}


void os_gfx_reset_frame_buffers(void){
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	OS_GFX_Context* ctx = os_gfx_get_current_global_context();
	lnx_ctx->current_frame_buffer = 0;

	for EachIndex(idx, 2){
		// free old shared frame buffer memory
		if(lnx_ctx->frame_buffer[idx]){
			xcb_shm_detach(lnx_ctx->connection, lnx_ctx->shm_info[idx].shmseg);
			free(lnx_ctx->frame_buffer[idx]);
			shmdt(lnx_ctx->shm_info[idx].shmaddr);
		}
		
		// make the shared memory region be the whole size of the screen
		U64 total_size = ctx->window.display_size.x * ctx->window.display_size.y * sizeof(ColorBGRA);
		
		// get the shared memory
		lnx_ctx->shm_info[idx].shmseg = xcb_generate_id(lnx_ctx->connection);
		lnx_ctx->shm_info[idx].shmid = shmget(IPC_PRIVATE, total_size, IPC_CREAT|0777);
		lnx_ctx->shm_info[idx].shmaddr = (void*) shmat(lnx_ctx->shm_info[idx].shmid, 0, 0);
		
		// create a new image
		lnx_ctx->frame_buffer[idx] = xcb_image_create_native(
			lnx_ctx->connection,
			ctx->window.display_size.x,
			ctx->window.display_size.y,
			XCB_IMAGE_FORMAT_Z_PIXMAP,
			lnx_ctx->screen->root_depth,
			lnx_ctx->shm_info[idx].shmaddr,
			total_size,
			lnx_ctx->shm_info[idx].shmaddr	
		);


		xcb_shm_attach(lnx_ctx->connection, 
		 		lnx_ctx->shm_info[idx].shmseg, 
		 		lnx_ctx->shm_info[idx].shmid, 
		 		0);

		xcb_flush(lnx_ctx->connection);

		shmctl(lnx_ctx->shm_info[idx].shmid, IPC_RMID, 0);	

	}
	
	ctx->window.frame_data = lnx_ctx->frame_buffer[lnx_ctx->current_frame_buffer]->data;
}

void os_gfx_swap_screen_buffer(void){
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	OS_GFX_Context* ctx = os_gfx_get_current_global_context();

	U32 frame = lnx_ctx->current_frame_buffer;
	xcb_image_t* image = lnx_ctx->frame_buffer[frame];
	
	U32 win_w = os_gfx_get_screen_width();	
	U32 win_h = os_gfx_get_screen_height();

	// put shm image to window
	xcb_shm_put_image(lnx_ctx->connection,
			lnx_ctx->window,
			lnx_ctx->graphics_ctx,
			image->width, image->height,
			0, 0,
			win_w, win_h,
			0, 0,
			image->depth,
			image->format,
			BASE_FALSE,
			lnx_ctx->shm_info[frame].shmseg,
			0);	

	// flush window
	xcb_flush(lnx_ctx->connection);
	
	// XOR the current frame buffer state
	lnx_ctx->current_frame_buffer ^= 1;

	ctx->window.frame_data = lnx_ctx->frame_buffer[lnx_ctx->current_frame_buffer]->data;
}

void os_gfx_paint_pixel(U32 width, U32 height, ColorRGBA c){
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	xcb_image_t* image = lnx_ctx->frame_buffer[lnx_ctx->current_frame_buffer];

	U32* pixels = (U32*) image->data;
	U32 stride = image->width;
	U32 color = color_rgba_to_bgra(c).c;

	Assert(0 <= width && width < image->width);
	Assert(0 <= height && height < image->height);

	if(width <= os_gfx_get_screen_width() && height <= os_gfx_get_screen_height()){
		pixels[height * stride + width] = color;
	}else{
		LogError("pixel write out of bounds");
	}
}

// window state options
void os_gfx_set_window_size(U32 w, U32 h){

	OS_GFX_Context* ctx = os_gfx_get_current_global_context();
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	
	// if window not resizeable don't do so
	if(!os_gfx_is_window_state(OS_GFX_WindowConfigFlag_Resizeable)) return;

	U32 width = ctx->window.screen_size.x;
	U32 height = ctx->window.screen_size.y;

	U32 min_width = ctx->window.screen_size_min.x;
	U32 max_width = ctx->window.screen_size_max.x;

	U32 min_height = ctx->window.screen_size_min.y;
	U32 max_height = ctx->window.screen_size_max.y;

	if(w != width || h != height){
		U32 new_width = Max(w, min_width);
		U32 new_height = Max(h, min_height);
		
		if(max_width > 0) new_width = Min(new_width, max_width);
		if(max_height > 0) new_height = Min(new_height, max_height);

		ctx->window.resized_last_frame = BASE_TRUE;	
		
		ctx->window.previous_screen_size.x = width;
		ctx->window.previous_screen_size.y = height;
		ctx->window.screen_size.x = new_width;
		ctx->window.screen_size.y = new_height;
	}
}
// set window options
void os_gfx_sync_window_config(void){
	//OS_GFX_Context* ctx = &glb_os_gfx_context;
	//OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	
	// update decorations
	//xcb_intern_atom_cookie_t motif_cookie = xcb_intern_atom()
}

void os_gfx_toggle_fullscreen(void){
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	
	// get the atoms for the state change
	char* state_message = "_NET_WM_STATE";
	U32 state_len = cstring_length(state_message);

	char* fullscreen_message = "_NET_WM_STATE_FULLSCREEN";
	U32 fullscreen_len = cstring_length(fullscreen_message); 

	xcb_intern_atom_cookie_t state_cookie = 
		xcb_intern_atom(lnx_ctx->connection, 0, state_len, state_message);

	xcb_intern_atom_cookie_t fs_cookie = 
		xcb_intern_atom(lnx_ctx->connection, 0, fullscreen_len, fullscreen_message);

	xcb_intern_atom_reply_t* state_reply = xcb_intern_atom_reply(lnx_ctx->connection, state_cookie, NULL);
	xcb_intern_atom_reply_t* fs_reply = xcb_intern_atom_reply(lnx_ctx->connection, fs_cookie, NULL);
	
	// prepare client message
	xcb_client_message_event_t event = {0};
	event.response_type = XCB_CLIENT_MESSAGE;
	event.format = 32;
	event.sequence = 0;
	event.window = lnx_ctx->window;
	event.type = state_reply->atom;

	B32 is_window_fs = os_gfx_is_window_state(OS_GFX_WindowConfigFlag_Fullscreen);
	event.data.data32[0] = !is_window_fs;
	event.data.data32[1] = fs_reply->atom;
	event.data.data32[2] = 0; 
	event.data.data32[3] = 1;
	
	if(is_window_fs) os_gfx_clear_window_state(OS_GFX_WindowConfigFlag_Fullscreen);
	else os_gfx_set_window_state(OS_GFX_WindowConfigFlag_Fullscreen);

	// send the event
	xcb_send_event(lnx_ctx->connection,
		0,
		lnx_ctx->screen->root,
		XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
		(const char*) &event);

	xcb_flush(lnx_ctx->connection);

	free(state_reply);
	free(fs_reply);
}

void os_gfx_maximize_window(void){
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	
	// get the atoms for the state change
	xcb_intern_atom_cookie_t state_cookie = xcb_intern_atom(lnx_ctx->connection, 0, 13, "_NET_WM_STATE");
	xcb_intern_atom_cookie_t horz_cookie = xcb_intern_atom(lnx_ctx->connection, 0, 28, "_NET_WM_STATE_MAXIMIZED_HORZ");
	xcb_intern_atom_cookie_t vert_cookie = xcb_intern_atom(lnx_ctx->connection, 0, 28, "_NET_WM_STATE_MAXIMIZED_VERT");

	xcb_intern_atom_reply_t* state_reply = xcb_intern_atom_reply(lnx_ctx->connection, state_cookie, NULL);
	xcb_intern_atom_reply_t* horz_reply = xcb_intern_atom_reply(lnx_ctx->connection, horz_cookie, NULL);
	xcb_intern_atom_reply_t* vert_reply = xcb_intern_atom_reply(lnx_ctx->connection, vert_cookie, NULL);
	
	// prepare client message
	xcb_client_message_event_t event = {0};
	event.response_type = XCB_CLIENT_MESSAGE;
	event.format = 32;
	event.sequence = 0;
	event.window = lnx_ctx->window;
	event.type = state_reply->atom;

	event.data.data32[0] = 1;
	event.data.data32[1] = horz_reply->atom;
	event.data.data32[2] = vert_reply->atom;
	event.data.data32[3] = 0;
	event.data.data32[4] = 0;

	// send the event
	xcb_send_event(lnx_ctx->connection,
		0,
		lnx_ctx->screen->root,
		XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
		(const char*) &event);

	xcb_flush(lnx_ctx->connection);

	free(state_reply);
	free(horz_reply);
	free(vert_reply);
}

void os_gfx_minimize_window(void){
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	
	// get the atoms for the state change
	xcb_intern_atom_cookie_t state_cookie = 
		xcb_intern_atom(lnx_ctx->connection, 0, cstring_length("WM_CHANGE_STATE") , "WM_CHANGE_STATE");

	xcb_intern_atom_reply_t* state_reply = 
		xcb_intern_atom_reply(lnx_ctx->connection, state_cookie, NULL);

	// prepare client message
	xcb_client_message_event_t event = {0};
	event.response_type = XCB_CLIENT_MESSAGE;
	event.format = 32;
	event.sequence = 0;
	event.window = lnx_ctx->window;
	event.type = state_reply->atom;

	event.data.data32[0] = 3;

	// send the event
	xcb_send_event(lnx_ctx->connection,
		0,
		lnx_ctx->screen->root,
		XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
		(const char*) &event);

	xcb_flush(lnx_ctx->connection);

	free(state_reply);
}

void os_gfx_restore_window(void){
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	
	// get the atoms for the state change
	char* client_message = "_NET_ACTIVE_WINDOW";
	xcb_intern_atom_cookie_t state_cookie = 
		xcb_intern_atom(lnx_ctx->connection, 0, cstring_length(client_message), client_message);

	xcb_intern_atom_reply_t* state_reply = 
		xcb_intern_atom_reply(lnx_ctx->connection, state_cookie, NULL);

	// prepare client message
	xcb_client_message_event_t event = {0};
	event.response_type = XCB_CLIENT_MESSAGE;
	event.format = 32;
	event.sequence = 0;
	event.window = lnx_ctx->window;
	event.type = state_reply->atom;

	event.data.data32[0] = 2;
	event.data.data32[1] = XCB_CURRENT_TIME;
	event.data.data32[2] = 0;
	
	// send the event
	xcb_send_event(lnx_ctx->connection,
		0,
		lnx_ctx->screen->root,
		XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
		(const char*) &event);

	xcb_flush(lnx_ctx->connection);
	free(state_reply);
}

void os_gfx_set_window_title(Str8 title){
	OS_GFX_Context* ctx = os_gfx_get_current_global_context();
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	
	ctx->window.title = title;
	
	// set window name
	xcb_change_property(lnx_ctx->connection,
			XCB_PROP_MODE_REPLACE,
			lnx_ctx->window,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING, 
			8,
			ctx->window.title.size,
			ctx->window.title.str);

	xcb_flush(lnx_ctx->connection);
}

void os_gfx_set_window_position(U32 x, U32 y){ 
	OS_GFX_Context* ctx = os_gfx_get_current_global_context();
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	
	// set window position 
	U32 values[] = {x, y};
	xcb_configure_window(lnx_ctx->connection,
			lnx_ctx->window,
			XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
			values);
	
	ctx->window.position = Pnt2_U32(x, y); 

	xcb_flush(lnx_ctx->connection);
}

void os_gfx_set_window_min_size(U32 width, U32 height){ 
	OS_GFX_Context* ctx = os_gfx_get_current_global_context();
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	
	Assert(0 <= width && width <= os_gfx_get_display_width());
	Assert(0 <= height && height <= os_gfx_get_display_height());

	xcb_size_hints_t hints;
	MemoryZeroStruct(&hints);

	hints.flags = XCB_ICCCM_SIZE_HINT_P_MAX_SIZE | XCB_ICCCM_SIZE_HINT_P_MIN_SIZE;
	hints.max_width = ctx->window.screen_size_max.x;
	hints.max_height = ctx->window.screen_size_max.y;
	hints.min_width = width;
	hints.min_height = height; 

	xcb_icccm_set_wm_normal_hints(lnx_ctx->connection, lnx_ctx->window, &hints);
	xcb_flush(lnx_ctx->connection);	

	os_gfx_set_window_state(OS_GFX_WindowConfigFlag_Resizeable);
	ctx->window.screen_size_min = Pnt2_U32(width, height); 
}

void os_gfx_set_window_max_size(U32 width, U32 height){ 
	OS_GFX_Context* ctx = os_gfx_get_current_global_context();
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();

	Assert(0 <= width && width <= os_gfx_get_display_width());
	Assert(0 <= height && height <= os_gfx_get_display_height());
	
	// send a icccm hint to set the window max size
	xcb_size_hints_t hints;
	MemoryZeroStruct(&hints);
	
	hints.flags = XCB_ICCCM_SIZE_HINT_P_MAX_SIZE | XCB_ICCCM_SIZE_HINT_P_MIN_SIZE;
	hints.max_width = width;
	hints.max_height = height;
	hints.min_width = ctx->window.screen_size_min.x;
	hints.min_height = ctx->window.screen_size_max.y;

	xcb_icccm_set_wm_normal_hints(lnx_ctx->connection, lnx_ctx->window, &hints);
	xcb_flush(lnx_ctx->connection);	

	os_gfx_set_window_state(OS_GFX_WindowConfigFlag_Resizeable);
	ctx->window.screen_size_max = Pnt2_U32(width, height); 
}

void os_gfx_set_window_focused(void){
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context();
	
	// get the atoms for the state change
	char* client_message = "_NET_ACTIVE_WINDOW";
	xcb_intern_atom_cookie_t state_cookie = xcb_intern_atom(lnx_ctx->connection, 0, cstring_length(client_message) , client_message);
	xcb_intern_atom_reply_t* state_reply = xcb_intern_atom_reply(lnx_ctx->connection, state_cookie, NULL);

	// prepare client message
	xcb_client_message_event_t event = {0};
	event.response_type = XCB_CLIENT_MESSAGE;
	event.format = 32;
	event.sequence = 0;
	event.window = lnx_ctx->window;
	event.type = state_reply->atom;

	event.data.data32[0] = 1;
	event.data.data32[1] = 1;
	event.data.data32[2] = 0;
	event.data.data32[3] = 0;
	event.data.data32[4] = 0;

	// send the event
	xcb_send_event(lnx_ctx->connection,
		0,
		lnx_ctx->screen->root,
		XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
		(const char*) &event);

	xcb_flush(lnx_ctx->connection);
	free(state_reply);
}

void os_gfx_poll_input_events(void){
	OS_LNX_GFX_Context* lnx_ctx = os_lnx_gfx_get_current_global_context(); 
	OS_GFX_Context* ctx = os_gfx_get_current_global_context();

	xcb_generic_event_t* e;
	while((e = xcb_poll_for_event(lnx_ctx->connection))){
		switch(e->response_type & ~0x80){
			case XCB_ENTER_NOTIFY:{
				ctx->input.mouse.cursor_on_screen = BASE_TRUE;
			}break;
			case XCB_LEAVE_NOTIFY:{
				ctx->input.mouse.cursor_on_screen = BASE_FALSE;
			}break;
			case XCB_DESTROY_NOTIFY:{
				ctx->window.should_close = BASE_TRUE;
			}break;
			case XCB_CLIENT_MESSAGE:{
				xcb_client_message_event_t* event = (xcb_client_message_event_t*)e;
				if(event->data.data32[0] == lnx_ctx->close_window_atom){
					ctx->window.should_close = BASE_TRUE;
				}
			}break;	
			case XCB_UNMAP_NOTIFY:{
				ctx->window.ready = BASE_FALSE;
			}break;
			case XCB_MAP_NOTIFY:{
				ctx->window.ready = BASE_TRUE;
			}break;	
			case XCB_FOCUS_IN:{
				os_gfx_clear_window_state(OS_GFX_WindowConfigFlag_Unfocused);
			}break;
			case XCB_FOCUS_OUT:{
				os_gfx_set_window_state(OS_GFX_WindowConfigFlag_Unfocused);
			}break;	
			case XCB_EXPOSE:{
				xcb_expose_event_t* event = (xcb_expose_event_t*)e;
				if(event->count == 0) os_gfx_swap_screen_buffer();	
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
			case XCB_KEYMAP_NOTIFY:{
				xcb_keymap_notify_event_t* event = (xcb_keymap_notify_event_t*)e;
			}break;
			case XCB_MAPPING_NOTIFY:{
				xcb_mapping_notify_event_t* event = (xcb_mapping_notify_event_t*)e;
			}break;	
			case XCB_VISIBILITY_NOTIFY:{
				xcb_visibility_notify_event_t* event = (xcb_visibility_notify_event_t*)e;
				if(event->state == XCB_VISIBILITY_UNOBSCURED){
					os_gfx_clear_window_state(OS_GFX_WindowConfigFlag_Hidden);
				}else if(event->state == XCB_VISIBILITY_PARTIALLY_OBSCURED){
					os_gfx_clear_window_state(OS_GFX_WindowConfigFlag_Hidden);
				}else if(event->state == XCB_VISIBILITY_FULLY_OBSCURED){
					os_gfx_set_window_state(OS_GFX_WindowConfigFlag_Hidden);
				}else{
					LogWarning("Unrecognized xcb visibility notify event type");
				}
			}break;
			case XCB_CONFIGURE_NOTIFY:{
				xcb_configure_notify_event_t* event = (xcb_configure_notify_event_t*)e;
				ctx->window.pending_size = Pnt2_U32(event->width, event->height);
				ctx->window.pending_resize = BASE_TRUE;

				ctx->window.previous_position.x = ctx->window.position.x;
				ctx->window.previous_position.y = ctx->window.position.y;
				ctx->window.position.x = event->x;
				ctx->window.position.y = event->y;
			}break;
			case XCB_GRAVITY_NOTIFY:{
				xcb_gravity_notify_event_t* event = (xcb_gravity_notify_event_t*)e;
				ctx->window.position.x = event->x;
				ctx->window.position.y = event->y;
			}break;
			case XCB_RESIZE_REQUEST:
			case XCB_NO_EXPOSURE:
			case XCB_GE_GENERIC:
			case XCB_REPARENT_NOTIFY:
			case XCB_PROPERTY_NOTIFY:
			case XCB_SELECTION_CLEAR:
			case XCB_SELECTION_REQUEST:
			case XCB_SELECTION_NOTIFY:
			case XCB_MAP_REQUEST:
			case XCB_CIRCULATE_NOTIFY:
			case XCB_CIRCULATE_REQUEST:
			case XCB_GRAPHICS_EXPOSURE:	
			case XCB_COLORMAP_NOTIFY:
			case XCB_CONFIGURE_REQUEST:
			case XCB_CREATE_NOTIFY:
			default:{
				LogWarning("Unrecognized xcb event type");
			}
		}
		free(e);
	}
}


