#ifndef BASE_OS_GFX_H
#define BASE_OS_GFX_H

#define MAX_KEYBOARD_KEYS 512 
#define MAX_KEY_PRESSED_QUEUE 16 
#define MAX_CHAR_PRESSED_QUEUE 16 

#define MAX_MOUSE_BUTTONS 8


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

typedef enum{
	OS_Event_None,
	OS_Event_MouseEnter,
	OS_Event_MouseLeave,
	OS_Event_Destroy,
	OS_Event_Client,
	OS_Event_Unmap,
	OS_Event_Map,
	OS_Event_GainFocus,
	OS_Event_LostFocus,
	OS_Event_Exposure,
	OS_Event_KeyPress,
	OS_Event_KeyRelease,
	OS_Event_ButtonPress,
	OS_Event_ButtonRelease,

	OS_Event_MouseMotion,
	OS_Event_KeymapChange,
	OS_Event_Mapping,
	OS_Event_Visibility,

	OS_Event_Configure,
	OS_Event_Gravity,

	XCB_REPARENT_NOTIFY
	XCB_PROPERTY_NOTIFY

	XCB_SELECTION_CLEAR
	XCB_SELECTION_REQUEST
	XCB_SELECTION_NOTIFY

	XCB_CIRCULATE_NOTIFY
	XCB_CIRCULATE_REQUEST
	XCB_GRAPHICS_EXPOSURE:
	XCB_COLORMAP_NOTIFY
	XCB_CONFIGURE_REQUEST
	XCB_CREATE_NOTIFY

} OS_Event;

typedef struct {
	struct{
		Str8 title;
		OS_GFX_WindowConfigFlag flags;
		B32 ready;
		B32 should_close;
		B32 resized_last_frame;
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

global OS_GFX_Context* glb_os_gfx_context;

// Open and closing window
OS_GFX_Context* os_gfx_init_window(Arena* arena, U32 x, U32 y, U32 width, U32 height, Str8 window_name);
void os_gfx_close_window(void);
void os_gfx_init_platform(Arena* arena);
void os_gfx_close_platform(void);
void os_gfx_set_global_context(OS_GFX_Context* ctx);	
OS_GFX_Context* os_gfx_get_current_context();


// Begin and end drawing
void os_gfx_begin(void);
void os_gfx_end(void);
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

typedef enum{
	OS_VInput_Modifier_None 		= 0,
	OS_VInput_Modifier_Shift 		= (1<<0),
	OS_VInput_Modifier_Function 		= (1<<1),
	OS_VInput_Modifier_Control 		= (1<<2),
	OS_VInput_Modifier_Super 		= (1<<3),
	OS_VInput_Modifier_Alt 			= (1<<4),
	OS_VInput_Modifier_CapsLock 		= (1<<5),
	OS_VInput_Modifier_COUNT
}OS_VInput_Modifier;

// virtual key x macro list
// variable name, string, virtual keycode, base modifiers
#define OS_VKEY_LIST \
	X(Tab,		"Tab", 		OS_VInput_Modifier_None) \
	X(Enter,	"Enter", 	OS_VInput_Modifier_None) \
	X(Escape,	"Escape", 	OS_VInput_Modifier_None) \
	X(Space,	"Space", 	OS_VInput_Modifier_None) \
	X(Backspace,	"Backspace", 	OS_VInput_Modifier_None) \
	X(CapsLock,	"CapsLock", 	OS_VInput_Modifier_None) \
	X(Super,	"Super", 	OS_VInput_Modifier_None) \

	X(UpArrow,	"UpArrow", 	OS_VInput_Modifier_None) \
	X(DownArrow,	"DownArrow", 	OS_VInput_Modifier_None) \
	X(LeftArrow,	"LeftArrow", 	OS_VInput_Modifier_None) \
	X(RightArrow,	"RightArrow", 	OS_VInput_Modifier_None) \

	X(Alt_R,	"Alt_R", 	OS_VInput_Modifier_None) \
	X(Alt_L,	"Alt_L", 	OS_VInput_Modifier_None) \
	X(Ctrl_R,	"Ctrl_R", 	OS_VInput_Modifier_None) \
	X(Ctrl_L,	"Ctrl_L", 	OS_VInput_Modifier_None) \
	X(Shift_R,	"Shift_R", 	OS_VInput_Modifier_None) \
	X(Shift_L,	"Shift_L", 	OS_VInput_Modifier_None) \
	X(Function,	"Function", 	OS_VInput_Modifier_None) \
	
	X(F1,		"F1", 		OS_VInput_Modifier_None) \
	X(F2,		"F2", 		OS_VInput_Modifier_None) \
	X(F3,		"F3", 		OS_VInput_Modifier_None) \
	X(F4,		"F4", 		OS_VInput_Modifier_None) \
	X(F5,		"F5", 		OS_VInput_Modifier_None) \
	X(F6,		"F6", 		OS_VInput_Modifier_None) \
	X(F7,		"F7", 		OS_VInput_Modifier_None) \
	X(F8,		"F8", 		OS_VInput_Modifier_None) \
	X(F9,		"F9", 		OS_VInput_Modifier_None) \
	X(F10,		"F10", 		OS_VInput_Modifier_None) \
	X(F11,		"F11", 		OS_VInput_Modifier_None) \
	X(F12,		"F12", 		OS_VInput_Modifier_None) \
	
	X(Insert,	"Ins", 		OS_VInput_Modifier_None) \
	X(Delete,	"Del", 		OS_VInput_Modifier_None) \
	X(PageDown,	"PgDn", 	OS_VInput_Modifier_None) \
	X(PageUp,	"PgUp", 	OS_VInput_Modifier_None) \
	X(Home,		"Home", 	OS_VInput_Modifier_None) \
	X(End,		"End", 		OS_VInput_Modifier_None) \
	
	X(A,		"A", 		OS_VInput_Modifier_Shift) \
	X(B,		"B", 		OS_VInput_Modifier_Shift) \
	X(C,		"C", 		OS_VInput_Modifier_Shift) \
	X(D,		"D", 		OS_VInput_Modifier_Shift) \
	X(E,		"E", 		OS_VInput_Modifier_Shift) \
	X(F,		"F", 		OS_VInput_Modifier_Shift) \
	X(G,		"G", 		OS_VInput_Modifier_Shift) \
	X(H,		"H", 		OS_VInput_Modifier_Shift) \
	X(I,		"I", 		OS_VInput_Modifier_Shift) \
	X(J,		"J", 		OS_VInput_Modifier_Shift) \
	X(K,		"K", 		OS_VInput_Modifier_Shift) \
	X(L,		"L", 		OS_VInput_Modifier_Shift) \
	X(M,		"M", 		OS_VInput_Modifier_Shift) \
	X(N,		"N", 		OS_VInput_Modifier_Shift) \
	X(O,		"O", 		OS_VInput_Modifier_Shift) \
	X(P,		"P", 		OS_VInput_Modifier_Shift) \
	X(Q,		"Q", 		OS_VInput_Modifier_Shift) \
	X(R,		"R", 		OS_VInput_Modifier_Shift) \
	X(S,		"S", 		OS_VInput_Modifier_Shift) \
	X(T,		"T", 		OS_VInput_Modifier_Shift) \
	X(U,		"U", 		OS_VInput_Modifier_Shift) \
	X(V,		"V", 		OS_VInput_Modifier_Shift) \
	X(W,		"W", 		OS_VInput_Modifier_Shift) \
	X(X,		"X", 		OS_VInput_Modifier_Shift) \
	X(Y,		"Y", 		OS_VInput_Modifier_Shift) \
	X(Z,		"Z", 		OS_VInput_Modifier_Shift) \

	X(a,		"a", 		OS_VInput_Modifier_None) \
	X(b,		"b", 		OS_VInput_Modifier_None) \
	X(c,		"c", 		OS_VInput_Modifier_None) \
	X(d,		"d", 		OS_VInput_Modifier_None) \
	X(e,		"e", 		OS_VInput_Modifier_None) \
	X(f,		"f", 		OS_VInput_Modifier_None) \
	X(g,		"g", 		OS_VInput_Modifier_None) \
	X(h,		"h", 		OS_VInput_Modifier_None) \
	X(i,		"i", 		OS_VInput_Modifier_None) \
	X(j,		"j", 		OS_VInput_Modifier_None) \
	X(k,		"k", 		OS_VInput_Modifier_None) \
	X(l,		"l", 		OS_VInput_Modifier_None) \
	X(m,		"m", 		OS_VInput_Modifier_None) \
	X(n,		"n", 		OS_VInput_Modifier_None) \
	X(o,		"o", 		OS_VInput_Modifier_None) \
	X(p,		"p", 		OS_VInput_Modifier_None) \
	X(q,		"q", 		OS_VInput_Modifier_None) \
	X(r,		"r", 		OS_VInput_Modifier_None) \
	X(s,		"s", 		OS_VInput_Modifier_None) \
	X(t,		"t", 		OS_VInput_Modifier_None) \
	X(u,		"u", 		OS_VInput_Modifier_None) \
	X(v,		"v", 		OS_VInput_Modifier_None) \
	X(w,		"w", 		OS_VInput_Modifier_None) \
	X(x,		"x", 		OS_VInput_Modifier_None) \
	X(y,		"y", 		OS_VInput_Modifier_None) \
	X(z,		"z", 		OS_VInput_Modifier_None) \

	X(1,		"1", 		OS_VInput_Modifier_None) \
	X(2,		"2", 		OS_VInput_Modifier_None) \
	X(3,		"3", 		OS_VInput_Modifier_None) \
	X(4,		"4", 		OS_VInput_Modifier_None) \
	X(5,		"5", 		OS_VInput_Modifier_None) \
	X(6,		"6", 		OS_VInput_Modifier_None) \
	X(7,		"7", 		OS_VInput_Modifier_None) \
	X(8,		"8", 		OS_VInput_Modifier_None) \
	X(9,		"9", 		OS_VInput_Modifier_None) \
	X(0,		"0", 		OS_VInput_Modifier_None) \

	X(Exclamation,	"!", 		OS_VInput_Modifier_Shift) \
	X(At,		"@", 		OS_VInput_Modifier_Shift) \
	X(Pound,	"#", 		OS_VInput_Modifier_Shift) \
	X(DollarSign,	"$", 		OS_VInput_Modifier_Shift) \
	X(Percent,	"%", 		OS_VInput_Modifier_Shift) \
	X(Carat,	"^", 		OS_VInput_Modifier_Shift) \
	X(And,		"&", 		OS_VInput_Modifier_Shift) \
	X(Star,		"*", 		OS_VInput_Modifier_Shift) \
	X(LeftParen,	"(", 		OS_VInput_Modifier_Shift) \
	X(RightParen,	")", 		OS_VInput_Modifier_Shift) \

	X(Comma,	",", 		OS_VInput_Modifier_None) \
	X(Period,	".", 		OS_VInput_Modifier_None) \
	X(ForwardSlash,	"/", 		OS_VInput_Modifier_None) \
	X(BackSlash,	"\\", 		OS_VInput_Modifier_None) \
	X(SemiColon,	";", 		OS_VInput_Modifier_None) \
	X(Quote,	"'", 		OS_VInput_Modifier_None) \
	X(BackQuote,	"`", 		OS_VInput_Modifier_None) \
	X(Minus,	"-", 		OS_VInput_Modifier_None) \
	X(Equal,	"=", 		OS_VInput_Modifier_None) \
	X(LeftBracket,	"[", 		OS_VInput_Modifier_None) \
	X(RightBracket,	"]", 		OS_VInput_Modifier_None) \

	X(LessThan,	"<", 		OS_VInput_Modifier_Shift) \
	X(GreaterThan,	">", 		OS_VInput_Modifier_Shift) \
	X(QuestionMark,	"?", 		OS_VInput_Modifier_Shift) \
	X(Colon,	":", 		OS_VInput_Modifier_Shift) \
	X(DoubleQuote,	"\"", 		OS_VInput_Modifier_Shift) \
	X(Bar,		"|", 		OS_VInput_Modifier_Shift) \
	X(Tilde,	"~", 		OS_VInput_Modifier_Shift) \
	X(Underscore,	"_", 		OS_VInput_Modifier_Shift) \
	X(Plus,		"+", 		OS_VInput_Modifier_Shift) \
	X(LeftBrace,	"{", 		OS_VInput_Modifier_Shift) \
	X(RightBrace,	"}", 		OS_VInput_Modifier_Shift) \


typedef enum{
#define X(name, str, mod) Glue(OS_VKey_, name),
	OS_VKEY_LIST
#undef X
	OS_VKey_COUNT,
}OS_VKey;

read_only U8* OS_VKey_Strings[] = {
#define X(name, str, mod) str,
	OS_VKEY_LIST
#undef X
};

read_only OS_VInput_Modifier OS_VKey_Modifiers[] = {
#define X(name, str, mod) mod,
	OS_VKEY_LIST
#undef X
};

OS_VKey os_gfx_get_resolved_key(OS_VKey key, OS_VInput_Modifier mod);
OS_VKey os_gfx_is_alpha_numeric(OS_VKey key, OS_VInput_Modifier mod);

// Input-related functions: keyboard
B32 os_gfx_is_key_pressed(OS_VKey key);                             // Check if a key has been pressed once
B32 os_gfx_is_key_pressed_repeat(OS_VKey key);                       // Check if a key has been pressed again
B32 os_gfx_is_key_down(OS_VKey key);                                // Check if a key is being pressed
B32 os_gfx_is_key_released(OS_VKey key);                            // Check if a key has been released once
B32 os_gfx_is_key_up(OS_VKey key);                                  // Check if a key is NOT being pressed
OS_VKey os_gfx_get_key_pressed(void);                                // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
OS_VKey os_gfx_get_char_pressed(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty
const U8* os_gfx_get_key_name(OS_VKey key);                        	// Get name of a QWERTY key on the current keyboard layout 
void os_gfx_set_exit_key(OS_VKey key, OS_VInput_Modifier mod);       // Set a custom key to exit program (default is ESC)


#define OS_VBUTTON_LIST \
	X(Mouse1,	"Mouse1", 	OS_VInput_Modifier_None) \
	X(Mouse2,	"Mouse2", 	OS_VInput_Modifier_None) \
	X(Mouse3,	"Mouse3", 	OS_VInput_Modifier_None) \
	X(Mouse4,	"Mouse4", 	OS_VInput_Modifier_None) \
	X(Mouse5,	"Mouse5", 	OS_VInput_Modifier_None) \
	X(MouseWheelUp,	"MouseWhlUp", 	OS_VInput_Modifier_None) \
	X(MouseWheelDn,	"MouseWhlDn", 	OS_VInput_Modifier_None) \

typedef enum{
#define X(name, str, mod) Glue(OS_VButton_, name),
	OS_VBUTTON_LIST
#undef X
	OS_VButton_COUNT,
}OS_VButton;

read_only U8* OS_VButton_Strings[] = {
#define X(name, str, mod) str,
	OS_VBUTTON_LIST
#undef X
};

read_only OS_VInput_Modifier OS_VMouse_Modifiers[] = {
#define X(name, str, mod) mod,
	OS_VBUTTON_LIST
#undef X
};
// Input-related functions: mouse
B32 os_gfx_is_mouse_button_pressed(OS_VButton button);                  // Check if a mouse button has been pressed once
B32 os_gfx_is_mouse_button_down(OS_VButton button);                     // Check if a mouse button is being pressed
B32 os_gfx_is_mouse_button_released(OS_VButton button);                 // Check if a mouse button has been released once
B32 os_gfx_is_mouse_button_up(OS_VButton button);                       // Check if a mouse button is NOT being pressed
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
