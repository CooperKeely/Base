#ifndef BASE_OS_GFX_H
#define BASE_OS_GFX_H

///////////////////////////////////////
/// cjk: Window API Definitions 

typedef U64 OS_GFX_WindowConfigFlag;
enum{
	OS_GFX_WindowConfigFlag_Fullscreen 	= (1<<0),
	OS_GFX_WindowConfigFlag_Resizeable	= (1<<1),
	OS_GFX_WindowConfigFlag_Undecorated	= (1<<2),
	OS_GFX_WindowConfigFlag_Hidden		= (1<<3),
	OS_GFX_WindowConfigFlag_Minimized	= (1<<4),
	OS_GFX_WindowConfigFlag_Maximized	= (1<<5),
	OS_GFX_WindowConfigFlag_Unfocused	= (1<<6),
	OS_GFX_WindowConfigFlag_TopMost		= (1<<7),
	OS_GFX_WindowConfigFlag_AlwaysRun	= (1<<8),
};


#define MAX_KEYBOARD_KEYS 512 
#define MAX_KEY_PRESSED_QUEUE 16 
#define MAX_CHAR_PRESSED_QUEUE 16 

#define MAX_MOUSE_BUTTONS 8

typedef struct {
	struct{
		Str8 title;
		OS_GFX_WindowConfigFlag flags;
		B32 ready;
		B32 should_close;
		B32 resized_last_frame;
		B32 pending_resize;
		B32 event_waiting;

		Pnt2U32 display_size;
		Pnt2U32 pending_size;
		Pnt2U32 screen_size;
		Pnt2U32 position;
		Pnt2U32 previous_screen_size;
		Pnt2U32 previous_position;

		Pnt2U32 screen_size_min;
		Pnt2U32 screen_size_max;

		void* frame_data; 
	}window;
	struct{
		struct{
			U32 exit_key;
			U8 current_key_state[MAX_KEYBOARD_KEYS];
			U8 previous_key_state[MAX_KEYBOARD_KEYS];
			
			U32 key_pressed_queue[MAX_KEY_PRESSED_QUEUE];
			U32 key_pressed_queue_count;

			U32 char_pressed_queue[MAX_CHAR_PRESSED_QUEUE];
			U32 char_pressed_queue_count;
		}keyboard;
		struct{
			Pnt2U32 offset;
			Pnt2U32 current_position;
			Pnt2U32 previous_position;
			Pnt2U32 locked_position;

			U32 cursor;
			B32 cursor_hidden;
			B32 cursor_locked;
			B32 cursor_on_screen;

			U8 current_button_state[MAX_MOUSE_BUTTONS];
			U8 previous_button_state[MAX_MOUSE_BUTTONS];
			Vec2F32 current_wheel_move;
			Vec2F32 previous_wheel_move;
		}mouse;
	}input;

	struct{
		U64 current; // in micro seconds
		U64 previous;

		F64 update;
		F64 draw;
		F64 frame;
		F64 target;

		U64 frame_counter;
	}time;
}OS_GFX_Context;

global OS_GFX_Context glb_os_gfx_context;

// Open and closing window
void os_gfx_init_window(U32 x, U32 y, U32 width, U32 height, Str8 window_name);
void os_gfx_close_window(void);
void os_gfx_init_platform(void);
void os_gfx_close_platform(void);

// Begin and end drawing
void os_gfx_begin_drawing(void);
void os_gfx_end_drawing(void);
void os_gfx_clear_background(ColorRGBA c);
void os_gfx_paint_pixel(U32 width, U32 height, ColorRGBA c);

// window helper functions
B32 os_gfx_window_should_close(void);
B32 os_gfx_is_window_ready(void);
B32 os_gfx_is_window_fullscreen(void);
B32 os_gfx_is_window_hidden(void);
B32 os_gfx_is_window_minimized(void);
B32 os_gfx_is_window_maximized(void);
B32 os_gfx_is_window_focused(void);
B32 os_gfx_is_window_resized(void);

// window state options
B32 os_gfx_is_window_state(U64 flag);
void os_gfx_set_window_state(OS_GFX_WindowConfigFlag flags);
void os_gfx_clear_window_state(OS_GFX_WindowConfigFlag flags);

// set window options
void os_gfx_toggle_fullscreen(void);
void os_gfx_maximize_window(void);
void os_gfx_minimize_window(void);
void os_gfx_restore_window(void);
void os_gfx_set_window_icon(); // TODO: (cjk): specify a parameter to take an image 
void os_gfx_set_window_title(Str8 title);
void os_gfx_set_window_position(U32 x, U32 y);
void os_gfx_set_window_min_size(U32 width, U32 height);
void os_gfx_set_window_max_size(U32 width, U32 height);
void os_gfx_set_window_size(U32 width, U32 height);
void os_gfx_set_window_focused(void);

// get window options
U32 os_gfx_get_screen_width(void);
U32 os_gfx_get_screen_height(void);
U32 os_gfx_get_display_width(void);
U32 os_gfx_get_display_height(void);
Pnt2U32 os_gfx_get_window_position(void);

// Cursor-related functions
void os_gfx_show_cursor(void);                                      
void os_gfx_hide_cursor(void);                                      
B32 os_gfx_is_cursor_hidden(void);                                  
void os_gfx_enable_cursor(void);                                    
void os_gfx_disable_cursor(void);                                   
B32 os_gfx_is_cursor_on_screen(void);                                

// Timing-related functions
void os_gfx_set_target_fps(U32 fps);                       // Set target FPS (maximum)
F64 os_gfx_get_frame_time(void);                         // Get time in seconds for last frame drawn (delta time)
F64 os_gfx_get_time(void);                             // Get elapsed time in seconds since Init_Window()
U32 os_gfx_get_fps(void);                                 // Get current FPS

void os_gfx_swap_screen_buffer(void);                      // Swap back buffer with front buffer (screen drawing)
void os_gfx_poll_input_events(void);                       // Register all input events
void* os_gfx_get_current_frame_buffer(void);
void os_gfx_reset_frame_buffers(void);

///////////////////////////////////////
/// cjk: Input Handeling API Definitions 

// Input-related functions: keyboard
B32 os_gfx_is_key_pressed(U32 key);                             // Check if a key has been pressed once
B32 os_gfx_is_key_pressed_repeat(U32 key);                       // Check if a key has been pressed again
B32 os_gfx_is_key_down(U32 key);                                // Check if a key is being pressed
B32 os_gfx_is_key_released(U32 key);                            // Check if a key has been released once
B32 os_gfx_is_key_up(U32 key);                                  // Check if a key is NOT being pressed
U32 os_gfx_get_key_pressed(void);                                // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
U32 os_gfx_get_char_pressed(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty
const U8* os_gfx_get_key_name(U32 key);                        // Get name of a QWERTY key on the current keyboard layout (eg returns string 'q' for KEY_A on an AZERTY keyboard)
void os_gfx_set_exit_key(U32 key);                               // Set a custom key to exit program (default is ESC)

// Input-related functions: mouse
B32 os_gfx_is_mouse_button_pressed(U32 button);                  // Check if a mouse button has been pressed once
B32 os_gfx_is_mouse_button_down(U32 button);                     // Check if a mouse button is being pressed
B32 os_gfx_is_mouse_button_released(U32 button);                 // Check if a mouse button has been released once
B32 os_gfx_is_mouse_button_up(U32 button);                       // Check if a mouse button is NOT being pressed
U32 os_gfx_get_mouse_x(void);                                    // Get mouse position X
U32 os_gfx_get_mouse_y(void);                                    // Get mouse position Y
Pnt2U32 os_gfx_get_mouse_position(void);                         // Get mouse position XY
Vec2F32 os_gfx_get_mouse_delta(void);                            // Get mouse delta between frames
void os_gfx_set_mouse_position(U32 x, U32 y);                    // Set mouse position XY
void os_gfx_set_mouse_offset(U32 offset_X, U32 offset_Y);          // Set mouse offset
F32 os_gfx_get_mouse_wheel_move(void);                          // Get mouse wheel movement for X or Y, whichever is larger
Vec2F32 os_gfx_get_mouse_wheel_move_v(void);                       // Get mouse wheel movement for both X and Y
void os_gfx_set_mouse_cursor(U32 cursor);                        // Set mouse cursor

#endif // BASE_OS_GFX
