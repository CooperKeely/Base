// Open and closing window
void os_gfx_init_window(U32 x, U32 y, U32 width, U32 height, Str8 window_name){
	Assert(!MemoryIsZeroStruct(&os_state)); // os must be inialized before using os gfx
	
	// make sure all memory is set to zero
	MemoryZeroStruct(&glb_os_gfx_context);
	OS_GFX_Context* ctx = &glb_os_gfx_context;	

	ctx->window.position = Pnt2_U32(x, y);	
	ctx->window.screen_size = Pnt2_U32(width, height);	
	ctx->window.title = window_name;

	os_gfx_init_platform();
	
	ctx->time.frame_counter = 0;
	ctx->window.should_close = BASE_FALSE;
}

void os_gfx_close_window(void){
	os_gfx_close_platform();
}

// Begin and end drawing
void os_gfx_begin_drawing(void){
	OS_GFX_Context* ctx = &glb_os_gfx_context;	

	ctx->time.current = os_now_microseconds();
	ctx->time.update = ctx->time.current - ctx->time.previous;
	ctx->time.previous = ctx->time.current;

	while (!os_gfx_is_window_ready()) os_gfx_poll_input_events();
}

void os_gfx_end_drawing(void){
	OS_GFX_Context* ctx = &glb_os_gfx_context;	
	
	// reset rsizing
	ctx->window.resized_last_frame = BASE_FALSE;

	os_gfx_poll_input_events();
	os_gfx_swap_screen_buffer();	
}

void os_gfx_clear_background(ColorRGBA c){

}

void* os_gfx_get_frame_buffer(void){
	return glb_os_gfx_context.window.frame_data;
}

// window helper functions
B32 os_gfx_window_should_close(void) { return glb_os_gfx_context.window.should_close; }
B32 os_gfx_is_window_ready(void){ return glb_os_gfx_context.window.ready; }
B32 os_gfx_is_window_resized(void){ return glb_os_gfx_context.window.resized_last_frame; }
B32 os_gfx_is_window_fullscreen(void){ return FLAG_IS_SET(glb_os_gfx_context.window.flags, OS_GFX_WindowConfigFlag_Fullscreen); }
B32 os_gfx_is_window_hidden(void){ return FLAG_IS_SET(glb_os_gfx_context.window.flags, OS_GFX_WindowConfigFlag_Hidden); }
B32 os_gfx_is_window_minimized(void){ return FLAG_IS_SET(glb_os_gfx_context.window.flags, OS_GFX_WindowConfigFlag_Minimized); }
B32 os_gfx_is_window_maximized(void){ return FLAG_IS_SET(glb_os_gfx_context.window.flags, OS_GFX_WindowConfigFlag_Maximized); }
B32 os_gfx_is_window_focused(void){ return !FLAG_IS_SET(glb_os_gfx_context.window.flags, OS_GFX_WindowConfigFlag_Unfocused); }

// window state options
B32 os_gfx_is_window_state(OS_GFX_WindowConfigFlag flag){ return FLAG_IS_SET(glb_os_gfx_context.window.flags, flag); }
void os_gfx_set_window_state(OS_GFX_WindowConfigFlag flags){ FLAG_SET(glb_os_gfx_context.window.flags, flags); }
void os_gfx_clear_window_state(OS_GFX_WindowConfigFlag flags){ FLAG_CLEAR(glb_os_gfx_context.window.flags, flags); }

// set window options
void os_gfx_toggle_fullscreen(void){

}

void os_gfx_maximize_window(void){

}

void os_gfx_minimize_window(void){

}

void os_gfx_restore_window(void){

}

void os_gfx_set_window_title(Str8 title){
	glb_os_gfx_context.window.title = title;
	// TODO: (cjk) send event to OS window system to update name
}

void os_gfx_set_window_position(U32 x, U32 y){ glb_os_gfx_context.window.position = Pnt2_U32(x, y); }
void os_gfx_set_window_min_size(U32 width, U32 height){ glb_os_gfx_context.window.screen_size_min = Pnt2_U32(width, height); }
void os_gfx_set_window_max_size(U32 width, U32 height){ glb_os_gfx_context.window.screen_size_max = Pnt2_U32(width, height); }

void os_gfx_set_window_focused(void){
}

// get window options
U32 os_gfx_get_screen_width(void){ return glb_os_gfx_context.window.screen_size.x; }
U32 os_gfx_get_screen_height(void){ return glb_os_gfx_context.window.screen_size.y; }
Pnt2U32 os_gfx_get_window_position(void){ return glb_os_gfx_context.window.position; }

// Cursor-related functions
void os_gfx_show_cursor(void){

}

void os_gfx_hide_cursor(void){

}

B32 os_gfx_is_cursor_hidden(void){ return glb_os_gfx_context.input.mouse.cursor_hidden; }
B32 os_gfx_is_cursor_on_screen(void){ return glb_os_gfx_context.input.mouse.cursor_on_screen; }

void os_gfx_enable_cursor(void){

}

void os_gfx_disable_cursor(void){

}

// Timing-related functions
void os_gfx_set_target_fps(U32 fps){
	if(fps == 0) glb_os_gfx_context.time.target = F32Lit(0.0);
	else glb_os_gfx_context.time.target = F32Lit(1.0)/(F32)fps;
} 

F64 os_gfx_get_frame_time(void){
	return glb_os_gfx_context.time.frame;
}

U32 os_gfx_get_fps(void){
	return U32Lit(60);	
}

///////////////////////////////////////
/// cjk: Input Handeling API Definitions 

// Input-related functions: keyboard
B32 os_gfx_is_key_pressed(U32 key){ NotImplemented; }
B32 os_gfx_is_key_pressed_repeat(U32 key){ NotImplemented; }
B32 os_gfx_is_key_down(U32 key){ NotImplemented; }
B32 os_gfx_is_key_released(U32 key){ NotImplemented; }
B32 os_gfx_is_key_up(U32 key){ NotImplemented; }
U32 os_gfx_get_key_pressed(void){ NotImplemented; }
U32 os_gfx_get_char_pressed(void){ NotImplemented; }
const U8* os_gfx_get_key_name(U32 key){ NotImplemented; }
void os_gfx_set_exit_key(U32 key){ NotImplemented; }

// Input-related functions: mouse
B32 os_gfx_is_mouse_button_pressed(U32 button){ NotImplemented; }
B32 os_gfx_is_mouse_button_down(U32 button){ NotImplemented; }
B32 os_gfx_is_mouse_button_released(U32 button){ NotImplemented; }
B32 os_gfx_is_mouse_button_up(U32 button){ NotImplemented; }

U32 os_gfx_get_mouse_x(void){
	return glb_os_gfx_context.input.mouse.current_position.x;
}

U32 os_gfx_get_mouse_y(void){
	return glb_os_gfx_context.input.mouse.current_position.y;
}

Pnt2U32 os_gfx_get_mouse_position(void){
	return glb_os_gfx_context.input.mouse.current_position;
}

Vec2F32 os_gfx_get_mouse_delta(void){
	U32 curr_x = glb_os_gfx_context.input.mouse.current_position.x;
	U32 curr_y = glb_os_gfx_context.input.mouse.current_position.y;
	U32 prev_x = glb_os_gfx_context.input.mouse.previous_position.x; 
	U32 prev_y = glb_os_gfx_context.input.mouse.previous_position.y; 
	return Vec2_F32(((F32)curr_x - (F32)prev_x), ((F32)curr_y - (F32)prev_y));
}

void os_gfx_set_mouse_position(U32 x, U32 y){
	glb_os_gfx_context.input.mouse.current_position = Pnt2_U32(x, y);
}

void os_gfx_set_mouse_offset(U32 offset_X, U32 offset_Y){
	glb_os_gfx_context.input.mouse.offset = Pnt2_U32(offset_X, offset_Y);
}

F32 os_gfx_get_mouse_wheel_move_x(void){ NotImplemented; }
F32 os_gfx_get_mouse_wheel_move_y(void){ NotImplemented; }
Vec2F32 os_gfx_get_mouse_wheel_move_v(void){ NotImplemented; }
void os_gfx_set_mouse_cursor(U32 cursor){ NotImplemented; }



