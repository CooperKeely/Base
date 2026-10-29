#ifndef WM_CORE_H
#define WM_CORE_H


// virtual key x macro list
// variable name, string, virtual keycode, base modifiers
#define WM_VKEY_TABLE \
	X(WM_VKey_Tab,		"Tab", 		WM_VInput_Modifier_None) \
	X(WM_VKey_Enter,	"Enter", 	WM_VInput_Modifier_None) \
	X(WM_VKey_Escape,	"Escape", 	WM_VInput_Modifier_None) \
	X(WM_VKey_Space,	"Space", 	WM_VInput_Modifier_None) \
	X(WM_VKey_Backspace,	"Backspace", 	WM_VInput_Modifier_None) \
	X(WM_VKey_CapsLock,	"CapsLock", 	WM_VInput_Modifier_None) \
	X(WM_VKey_Super,	"Super", 	WM_VInput_Modifier_None) \
								 \
	X(WM_VKey_UpArrow,	"UpArrow", 	WM_VInput_Modifier_None) \
	X(WM_VKey_DownArrow,	"DownArrow", 	WM_VInput_Modifier_None) \
	X(WM_VKey_LeftArrow,	"LeftArrow", 	WM_VInput_Modifier_None) \
	X(WM_VKey_RightArrow,	"RightArrow", 	WM_VInput_Modifier_None) \
								 \
	X(WM_VKey_Alt_R,	"Alt_R", 	WM_VInput_Modifier_None) \
	X(WM_VKey_Alt_L,	"Alt_L", 	WM_VInput_Modifier_None) \
	X(WM_VKey_Ctrl_R,	"Ctrl_R", 	WM_VInput_Modifier_None) \
	X(WM_VKey_Ctrl_L,	"Ctrl_L", 	WM_VInput_Modifier_None) \
	X(WM_VKey_Shift_R,	"Shift_R", 	WM_VInput_Modifier_None) \
	X(WM_VKey_Shift_L,	"Shift_L", 	WM_VInput_Modifier_None) \
	X(WM_VKey_Function,	"Function", 	WM_VInput_Modifier_None) \
								 \
	X(WM_VKey_F1,		"F1", 		WM_VInput_Modifier_Function) \
	X(WM_VKey_F2,		"F2", 		WM_VInput_Modifier_Function) \
	X(WM_VKey_F3,		"F3", 		WM_VInput_Modifier_Function) \
	X(WM_VKey_F4,		"F4", 		WM_VInput_Modifier_Function) \
	X(WM_VKey_F5,		"F5", 		WM_VInput_Modifier_Function) \
	X(WM_VKey_F6,		"F6", 		WM_VInput_Modifier_Function) \
	X(WM_VKey_F7,		"F7", 		WM_VInput_Modifier_Function) \
	X(WM_VKey_F8,		"F8", 		WM_VInput_Modifier_Function) \
	X(WM_VKey_F9,		"F9", 		WM_VInput_Modifier_Function) \
	X(WM_VKey_F10,		"F10", 		WM_VInput_Modifier_Function) \
	X(WM_VKey_F11,		"F11", 		WM_VInput_Modifier_Function) \
	X(WM_VKey_F12,		"F12", 		WM_VInput_Modifier_Function) \
								 \
	X(WM_VKey_Insert,	"Ins", 		WM_VInput_Modifier_None) \
	X(WM_VKey_Delete,	"Del", 		WM_VInput_Modifier_None) \
	X(WM_VKey_PageDown,	"PgDn", 	WM_VInput_Modifier_None) \
	X(WM_VKey_PageUp,	"PgUp", 	WM_VInput_Modifier_None) \
	X(WM_VKey_Home,		"Home", 	WM_VInput_Modifier_None) \
	X(WM_VKey_End,		"End", 		WM_VInput_Modifier_None) \
									\
	X(WM_VKey_A,		"A", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_B,		"B", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_C,		"C", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_D,		"D", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_E,		"E", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_F,		"F", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_G,		"G", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_H,		"H", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_I,		"I", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_J,		"J", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_K,		"K", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_L,		"L", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_M,		"M", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_N,		"N", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_O,		"O", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_P,		"P", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Q,		"Q", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_R,		"R", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_S,		"S", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_T,		"T", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_U,		"U", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_V,		"V", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_W,		"W", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_X,		"X", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Y,		"Y", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Z,		"Z", 		WM_VInput_Modifier_Shift) \
								  \
	X(WM_VKey_a,		"a", 		WM_VInput_Modifier_None) \
	X(WM_VKey_b,		"b", 		WM_VInput_Modifier_None) \
	X(WM_VKey_c,		"c", 		WM_VInput_Modifier_None) \
	X(WM_VKey_d,		"d", 		WM_VInput_Modifier_None) \
	X(WM_VKey_e,		"e", 		WM_VInput_Modifier_None) \
	X(WM_VKey_f,		"f", 		WM_VInput_Modifier_None) \
	X(WM_VKey_g,		"g", 		WM_VInput_Modifier_None) \
	X(WM_VKey_h,		"h", 		WM_VInput_Modifier_None) \
	X(WM_VKey_i,		"i", 		WM_VInput_Modifier_None) \
	X(WM_VKey_j,		"j", 		WM_VInput_Modifier_None) \
	X(WM_VKey_k,		"k", 		WM_VInput_Modifier_None) \
	X(WM_VKey_l,		"l", 		WM_VInput_Modifier_None) \
	X(WM_VKey_m,		"m", 		WM_VInput_Modifier_None) \
	X(WM_VKey_n,		"n", 		WM_VInput_Modifier_None) \
	X(WM_VKey_o,		"o", 		WM_VInput_Modifier_None) \
	X(WM_VKey_p,		"p", 		WM_VInput_Modifier_None) \
	X(WM_VKey_q,		"q", 		WM_VInput_Modifier_None) \
	X(WM_VKey_r,		"r", 		WM_VInput_Modifier_None) \
	X(WM_VKey_s,		"s", 		WM_VInput_Modifier_None) \
	X(WM_VKey_t,		"t", 		WM_VInput_Modifier_None) \
	X(WM_VKey_u,		"u", 		WM_VInput_Modifier_None) \
	X(WM_VKey_v,		"v", 		WM_VInput_Modifier_None) \
	X(WM_VKey_w,		"w", 		WM_VInput_Modifier_None) \
	X(WM_VKey_x,		"x", 		WM_VInput_Modifier_None) \
	X(WM_VKey_y,		"y", 		WM_VInput_Modifier_None) \
	X(WM_VKey_z,		"z", 		WM_VInput_Modifier_None) \
								 \
	X(WM_VKey_1,		"1", 		WM_VInput_Modifier_None) \
	X(WM_VKey_2,		"2", 		WM_VInput_Modifier_None) \
	X(WM_VKey_3,		"3", 		WM_VInput_Modifier_None) \
	X(WM_VKey_4,		"4", 		WM_VInput_Modifier_None) \
	X(WM_VKey_5,		"5", 		WM_VInput_Modifier_None) \
	X(WM_VKey_6,		"6", 		WM_VInput_Modifier_None) \
	X(WM_VKey_7,		"7", 		WM_VInput_Modifier_None) \
	X(WM_VKey_8,		"8", 		WM_VInput_Modifier_None) \
	X(WM_VKey_9,		"9", 		WM_VInput_Modifier_None) \
	X(WM_VKey_0,		"0", 		WM_VInput_Modifier_None) \
								 \
	X(WM_VKey_Exclamation,	"!", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_At,		"@", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Pound,	"#", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_DollarSign,	"$", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Percent,	"%", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Carat,	"^", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_And,		"&", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Star,		"*", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_LeftParen,	"(", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_RightParen,	")", 		WM_VInput_Modifier_Shift) \
								  \
	X(WM_VKey_Comma,	",", 		WM_VInput_Modifier_None) \
	X(WM_VKey_Period,	".", 		WM_VInput_Modifier_None) \
	X(WM_VKey_ForwardSlash,	"/", 		WM_VInput_Modifier_None) \
	X(WM_VKey_BackSlash,	"\\", 		WM_VInput_Modifier_None) \
	X(WM_VKey_SemiColon,	";", 		WM_VInput_Modifier_None) \
	X(WM_VKey_Quote,	"'", 		WM_VInput_Modifier_None) \
	X(WM_VKey_BackQuote,	"`", 		WM_VInput_Modifier_None) \
	X(WM_VKey_Minus,	"-", 		WM_VInput_Modifier_None) \
	X(WM_VKey_Equal,	"=", 		WM_VInput_Modifier_None) \
	X(WM_VKey_LeftBracket,	"[", 		WM_VInput_Modifier_None) \
	X(WM_VKey_RightBracket,	"]", 		WM_VInput_Modifier_None) \
								 \
	X(WM_VKey_LessThan,	"<", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_GreaterThan,	">", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_QuestionMark,	"?", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Colon,	":", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_DoubleQuote,	"\"", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Bar,		"|", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Tilde,	"~", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Underscore,	"_", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_Plus,		"+", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_LeftBrace,	"{", 		WM_VInput_Modifier_Shift) \
	X(WM_VKey_RightBrace,	"}", 		WM_VInput_Modifier_Shift) \


///////////////////////////////////////
/// cjk: WM_Input API


typedef enum{
	WM_VInput_Modifier_None 	= 0,
	WM_VInput_Modifier_Shift 	= (1<<0),
	WM_VInput_Modifier_Function 	= (1<<1),
	WM_VInput_Modifier_Control 	= (1<<2),
	WM_VInput_Modifier_Super 	= (1<<3),
	WM_VInput_Modifier_Alt 		= (1<<4),
	WM_VInput_Modifier_CapsLock 	= (1<<5),
	WM_VInput_Modifier_COUNT
}WM_VInput_Modifier;

typedef enum{
#define X(name, str, mod) name,
	WM_VKEY_TABLE 
#undef X
	WM_VKey_COUNT,
}WM_VKey;

read_only Str8 WM_VKey_Strings[] = {
#define X(name, str, mod) Str8Comp(str),
	WM_VKEY_TABLE	
#undef X
};

read_only WM_VInput_Modifier WM_VKey_Modifiers[] = {
#define X(name, str, mod) mod,
	WM_VKEY_TABLE	
#undef X
};

///////////////////////////////////////
/// cjk: WM_Mouse API


#define WM_VBUTTON_TABLE \
	X(Mouse1,	"Mouse1", 	WM_VInput_Modifier_None) \
	X(Mouse2,	"Mouse2", 	WM_VInput_Modifier_None) \
	X(Mouse3,	"Mouse3", 	WM_VInput_Modifier_None) \
	X(Mouse4,	"Mouse4", 	WM_VInput_Modifier_None) \
	X(Mouse5,	"Mouse5", 	WM_VInput_Modifier_None) \
	X(MouseWheelUp,	"MouseWhlUp", 	WM_VInput_Modifier_None) \
	X(MouseWheelDn,	"MouseWhlDn", 	WM_VInput_Modifier_None) \

typedef enum{
#define X(name, str, mod) Glue(WM_VButton_, name),
	WM_VBUTTON_TABLE
#undef X
	WM_VButton_COUNT,
}WM_VButton;

read_only Str8 WM_VButton_Strings[] = {
#define X(name, str, mod) Str8Comp(str),
	WM_VBUTTON_TABLE
#undef X
};

read_only WM_VInput_Modifier WM_VMouse_Modifiers[] = {
#define X(name, str, mod) mod,
	WM_VBUTTON_TABLE
#undef X
};


///////////////////////////////////////
/// cjk: WM_Event api 

typedef enum{
	WM_EventType_None,
	
	// Window related events
	WM_EventType_Window_Shown,
	WM_EventType_Window_Hidden,
	WM_EventType_Window_Redraw,
	WM_EventType_Window_FocusGained,
	WM_EventType_Window_FocusLost,
	WM_EventType_Window_Close,
	WM_EventType_Window_Resize,
	WM_EventType_Window_Move,

	// Keyboard related events
	WM_EventType_Key_Press,
	WM_EventType_Key_Release,
	WM_EventType_Key_Repeat,
	WM_EventType_Key_MapChange,
	

	// Mouse related events
	WM_EventType_Mouse_EnterWindow,
	WM_EventType_Mouse_LeaveWindow,
	WM_EventType_Mouse_Motion,
	WM_EventType_Mouse_Scroll,
	WM_EventType_Mouse_Press,
	WM_EventType_Mouse_Release,

	// System updates
	WM_EventType_System_ClipboardUpdate,
	WM_EventType_System_DPIChange,
	WM_EventType_System_DropFile,
	WM_EventType_System_Quit,

	WM_EventType_COUNT
} WM_EventType;


typedef struct WM_Event WM_Event;
struct WM_Event{
	WM_Event* next;
	WM_Event* prev;

	WM_EventType type;
	U64 time_stamp;	
	
	// mouse related event data 
	Vec2F64 mouse_location;
	Vec2F64 mouse_scroll_vector;
	WM_VButton mouse_button;
	WM_VInput_Modifier mouse_mod;
	
	// keybaord related event data 
	WM_VKey key;
	B32 key_is_repeat;
	WM_VInput_Modifier key_mod;

	// window related event data
	Vec2F64 window_location;
	Vec2F64 window_size;

	// System related event data
	OS_Handle system_clip_board;
	OS_Handle system_file_drop;
	F64 system_new_dpi;
};

///////////////////////////////////////
/// cjk: Window Manager API
	
typedef U64 WM_ConfigFlag;
enum{
	WM_ConfigFlag_None		= 0,
	WM_ConfigFlag_Fullscreen 	= (1<<0),
	WM_ConfigFlag_Resizeable	= (1<<1),
	WM_ConfigFlag_Undecorated	= (1<<2),
	WM_ConfigFlag_Hidden		= (1<<3),
	WM_ConfigFlag_Minimized		= (1<<4),
	WM_ConfigFlag_Maximized		= (1<<5),
	WM_ConfigFlag_Unfocused		= (1<<6),
	WM_ConfigFlag_TopMost		= (1<<7),
	WM_ConfigFlag_AlwaysRun		= (1<<8),
	WM_ConfigFlag_COUNT
};

typedef struct{
	F32 double_click_time;
	F32 caret_blink_time;
	F32 default_refresh_rate; 
}WM_ConfigValues;

// cjk: lightweight handles for os specific opaque data types
typedef struct Handle WM_Window;
typedef struct Handle WM_Monitor;
typedef struct Handle WM_Display;

///////////////////////////////////////
/// cjk: Native Handle Export
	

typedef U64 WM_NativeWindowType;
enum {
	WM_NativeWindowType_NONE = 0,
	WM_NativeWindowType_X11,
	WM_NativeWindowType_Wayland,
	WM_NativeWindowType_Win32,
	WM_NativeWindowType_Cocoa,
	WM_NativeWindowType_COUNT
};

typedef struct {
	WM_NativeWindowType type;

	union{
		struct{
			void* display;		// Display*
			U64 surface;		// Window
		} x11;

		struct{
			void* display;		// struct wl_display*
			void* surface;		// struct wl_surface*
		} wayland;
	
		struct {
			void* hinstance; 	// HINSTANCE
			void* hwnd;		// HWND
			void* hdc;		// HDC
		} win32;

		struct {
			void* ns_window;	// NSWindow*
			void* ns_view;		// NSView*
		} cocoa;
	};

}WM_NativeHandles;


// cjk: graphics glue layer export helper
void* wm_get_native_handles(Arena* arena, WM_Window window);


// cjk: creates a connection to the OS window manager
void wm_create_connection(WM_ConfigFlag flags);
void wm_close_connection();

// cjk: creates a new window with the window manager 
WM_Window wm_create_window(Arena* arena, F32 width, F32 height, Str8 window_name);
void wm_close_window(WM_Window window);

// cjk: window functions
void wm_set_window_title(WM_Window window, Str8 window_name);
void wm_is_window_focused(WM_Window window);
void wm_set_window_focus(WM_Window window);
void wm_is_window_fullscreen(WM_Window window);
void wm_set_window_fullscreen(WM_Window window);
void wm_is_window_maximized(WM_Window window);
void wm_set_window_maximized(WM_Window window);
void wm_is_window_minimized(WM_Window window);
void wm_set_window_minimized(WM_Window window);
void wm_bring_window_to_front(WM_Window window);
void wm_set_window_monitor(WM_Window window, WM_Monitor monitor);

// cjk: monitor functions
// TODO: add monitor functions

/// cjk: Event functions
WM_Event* wm_get_event_list(WM_Window window, Arena* arena);

#endif // WM_CORE_H 
