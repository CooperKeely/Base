#ifndef BASE_OS_GFX_H
#define BASE_OS_GFX_H

///////////////////////////////////////
/// cjk: Window API Definitions 

typedef U64 OS_GFX_EventFlag;
typedef U64 OS_GFX_WindowStateFlag;

typedef struct {
	struct{
		Str8 title;
		OS_GFX_WindowStateFlag flags;
		B32 ready;
		B32 should_close;
		B32 resized_last_frame;
		B32 event_waiting;
		
		Vec2U32 display_size;
		Vec2U32 screen_size;
		Point2U32 position;
		Vec2U32 previous_screen_size;
		Point2U32 prvious_position;
		
		Vec2U32 render_size;

		Vec2U32 screen_size_min;
		Vec2U32 screen_size_max;
	}window;
	struct{
		struct{
			U32 exit_key;
			U8 current_key_state[MAX_KEYBOARD_KEYS];
			U8 previous_key_state[MAX_KEYBOARD_KEYS];

		}keyboard;
		struct{

		}mouse;






	}input;

}OS_GFX_Context ;


// Open and closing window
void os_gfx_init_window(U32 x, U32 y, U32 width, U32 height, Str8 window_name);
void os_gfx_close_window(void);

// Begin and end drawing
void os_gfx_begin_drawing(void);
void os_gfx_end_drawing(void);
void os_gfx_clear_background(ColorRGBA c);

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
B32 os_gfx_set_window_state(OS_GFX_WindowStateFlag flags);
B32 os_gfx_clear_window_state(OS_GFX_WindowStateFlag flags);

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
Vec2U32 os_gfx_get_window_position(void);

// Cursor-related functions
void os_gfx_show_cursor(void);                                      
void os_gfx_hide_cursor(void);                                      
bool os_gfx_is_cursor_hidden(void);                                  
void os_gfx_enable_cursor(void);                                    
void os_gfx_disable_cursor(void);                                   
bool os_gfx_is_cursor_on_screen(void);                                

// Timing-related functions
void os_gfx_set_target_fps(int fps);                       // Set target FPS (maximum)
float os_gfx_get_frame_time(void);                         // Get time in seconds for last frame drawn (delta time)
double os_gfx_get_time(void);                             // Get elapsed time in seconds since Init_Window()
int os_gfx_get_fps(void);                                 // Get current FPS

void os_gfx_swap_screen_buffer(void);                      // Swap back buffer with front buffer (screen drawing)
void os_gfx_poll_input_events(void);                       // Register all input events
void os_gfx_wait_time(double seconds);                    // Wait for some time (halt program execution)

///////////////////////////////////////
/// cjk: Input Handeling API Definitions 

// Input-related functions: keyboard
bool os_gfx_is_key_pressed(int key);                             // Check if a key has been pressed once
bool os_gfx_is_key_pressed_repeat(int key);                       // Check if a key has been pressed again
bool os_gfx_is_key_down(int key);                                // Check if a key is being pressed
bool os_gfx_is_key_released(int key);                            // Check if a key has been released once
bool os_gfx_is_key_up(int key);                                  // Check if a key is NOT being pressed
int os_gfx_get_key_pressed(void);                                // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
int os_gfx_get_char_pressed(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty
const char* os_gfx_get_key_name(int key);                        // Get name of a QWERTY key on the current keyboard layout (eg returns string 'q' for KEY_A on an AZERTY keyboard)
void os_gfx_set_exit_key(int key);                               // Set a custom key to exit program (default is ESC)

// Input-related functions: mouse
bool os_gfx_is_mouse_button_pressed(int button);                  // Check if a mouse button has been pressed once
bool os_gfx_is_mouse_button_down(int button);                     // Check if a mouse button is being pressed
bool os_gfx_is_mouse_button_released(int button);                 // Check if a mouse button has been released once
bool os_gfx_is_mouse_button_up(int button);                       // Check if a mouse button is NOT being pressed
int os_gfx_get_mouse_x(void);                                    // Get mouse position X
int os_gfx_get_mouse_y(void);                                    // Get mouse position Y
Vector2 os_gfx_get_mouse_position(void);                         // Get mouse position XY
Vector2 os_gfx_get_mouse_delta(void);                            // Get mouse delta between frames
void os_gfx_set_mouse_position(int x, int y);                    // Set mouse position XY
void os_gfx_set_mouse_offset(int offset_X, int offset_Y);          // Set mouse offset
void os_gfx_set_mouse_scale(float scale_X, float scale_Y);         // Set mouse scaling
float os_gfx_get_mouse_wheel_move(void);                          // Get mouse wheel movement for X or Y, whichever is larger
Vector2 os_gfx_get_mouse_wheel_move_v(void);                       // Get mouse wheel movement for both X and Y
void os_gfx_set_mouse_cursor(int cursor);                        // Set mouse cursor

#endif // BASE_OS_GFX
