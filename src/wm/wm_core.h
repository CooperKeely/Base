#ifndef WM_CORE_H
#define WM_CORE_H


// virtual key x macro list
// variable name, string, virtual keycode, base modifiers
#define WM_VKEY_TABLE \
	X(Tab,		"Tab", 		WM_VInput_Modifier_None) \
	X(Enter,	"Enter", 	WM_VInput_Modifier_None) \
	X(Escape,	"Escape", 	WM_VInput_Modifier_None) \
	X(Space,	"Space", 	WM_VInput_Modifier_None) \
	X(Backspace,	"Backspace", 	WM_VInput_Modifier_None) \
	X(CapsLock,	"CapsLock", 	WM_VInput_Modifier_None) \
	X(Super,	"Super", 	WM_VInput_Modifier_None) \
								 \
	X(UpArrow,	"UpArrow", 	WM_VInput_Modifier_None) \
	X(DownArrow,	"DownArrow", 	WM_VInput_Modifier_None) \
	X(LeftArrow,	"LeftArrow", 	WM_VInput_Modifier_None) \
	X(RightArrow,	"RightArrow", 	WM_VInput_Modifier_None) \
								 \
	X(Alt_R,	"Alt_R", 	WM_VInput_Modifier_None) \
	X(Alt_L,	"Alt_L", 	WM_VInput_Modifier_None) \
	X(Ctrl_R,	"Ctrl_R", 	WM_VInput_Modifier_None) \
	X(Ctrl_L,	"Ctrl_L", 	WM_VInput_Modifier_None) \
	X(Shift_R,	"Shift_R", 	WM_VInput_Modifier_None) \
	X(Shift_L,	"Shift_L", 	WM_VInput_Modifier_None) \
	X(Function,	"Function", 	WM_VInput_Modifier_None) \
								 \
	X(F1,		"F1", 		WM_VInput_Modifier_Function) \
	X(F2,		"F2", 		WM_VInput_Modifier_Function) \
	X(F3,		"F3", 		WM_VInput_Modifier_Function) \
	X(F4,		"F4", 		WM_VInput_Modifier_Function) \
	X(F5,		"F5", 		WM_VInput_Modifier_Function) \
	X(F6,		"F6", 		WM_VInput_Modifier_Function) \
	X(F7,		"F7", 		WM_VInput_Modifier_Function) \
	X(F8,		"F8", 		WM_VInput_Modifier_Function) \
	X(F9,		"F9", 		WM_VInput_Modifier_Function) \
	X(F10,		"F10", 		WM_VInput_Modifier_Function) \
	X(F11,		"F11", 		WM_VInput_Modifier_Function) \
	X(F12,		"F12", 		WM_VInput_Modifier_Function) \
								 \
	X(Insert,	"Ins", 		WM_VInput_Modifier_None) \
	X(Delete,	"Del", 		WM_VInput_Modifier_None) \
	X(PageDown,	"PgDn", 	WM_VInput_Modifier_None) \
	X(PageUp,	"PgUp", 	WM_VInput_Modifier_None) \
	X(Home,		"Home", 	WM_VInput_Modifier_None) \
	X(End,		"End", 		WM_VInput_Modifier_None) \
									\
	X(A,		"A", 		WM_VInput_Modifier_Shift) \
	X(B,		"B", 		WM_VInput_Modifier_Shift) \
	X(C,		"C", 		WM_VInput_Modifier_Shift) \
	X(D,		"D", 		WM_VInput_Modifier_Shift) \
	X(E,		"E", 		WM_VInput_Modifier_Shift) \
	X(F,		"F", 		WM_VInput_Modifier_Shift) \
	X(G,		"G", 		WM_VInput_Modifier_Shift) \
	X(H,		"H", 		WM_VInput_Modifier_Shift) \
	X(I,		"I", 		WM_VInput_Modifier_Shift) \
	X(J,		"J", 		WM_VInput_Modifier_Shift) \
	X(K,		"K", 		WM_VInput_Modifier_Shift) \
	X(L,		"L", 		WM_VInput_Modifier_Shift) \
	X(M,		"M", 		WM_VInput_Modifier_Shift) \
	X(N,		"N", 		WM_VInput_Modifier_Shift) \
	X(O,		"O", 		WM_VInput_Modifier_Shift) \
	X(P,		"P", 		WM_VInput_Modifier_Shift) \
	X(Q,		"Q", 		WM_VInput_Modifier_Shift) \
	X(R,		"R", 		WM_VInput_Modifier_Shift) \
	X(S,		"S", 		WM_VInput_Modifier_Shift) \
	X(T,		"T", 		WM_VInput_Modifier_Shift) \
	X(U,		"U", 		WM_VInput_Modifier_Shift) \
	X(V,		"V", 		WM_VInput_Modifier_Shift) \
	X(W,		"W", 		WM_VInput_Modifier_Shift) \
	X(X,		"X", 		WM_VInput_Modifier_Shift) \
	X(Y,		"Y", 		WM_VInput_Modifier_Shift) \
	X(Z,		"Z", 		WM_VInput_Modifier_Shift) \
								  \
	X(a,		"a", 		WM_VInput_Modifier_None) \
	X(b,		"b", 		WM_VInput_Modifier_None) \
	X(c,		"c", 		WM_VInput_Modifier_None) \
	X(d,		"d", 		WM_VInput_Modifier_None) \
	X(e,		"e", 		WM_VInput_Modifier_None) \
	X(f,		"f", 		WM_VInput_Modifier_None) \
	X(g,		"g", 		WM_VInput_Modifier_None) \
	X(h,		"h", 		WM_VInput_Modifier_None) \
	X(i,		"i", 		WM_VInput_Modifier_None) \
	X(j,		"j", 		WM_VInput_Modifier_None) \
	X(k,		"k", 		WM_VInput_Modifier_None) \
	X(l,		"l", 		WM_VInput_Modifier_None) \
	X(m,		"m", 		WM_VInput_Modifier_None) \
	X(n,		"n", 		WM_VInput_Modifier_None) \
	X(o,		"o", 		WM_VInput_Modifier_None) \
	X(p,		"p", 		WM_VInput_Modifier_None) \
	X(q,		"q", 		WM_VInput_Modifier_None) \
	X(r,		"r", 		WM_VInput_Modifier_None) \
	X(s,		"s", 		WM_VInput_Modifier_None) \
	X(t,		"t", 		WM_VInput_Modifier_None) \
	X(u,		"u", 		WM_VInput_Modifier_None) \
	X(v,		"v", 		WM_VInput_Modifier_None) \
	X(w,		"w", 		WM_VInput_Modifier_None) \
	X(x,		"x", 		WM_VInput_Modifier_None) \
	X(y,		"y", 		WM_VInput_Modifier_None) \
	X(z,		"z", 		WM_VInput_Modifier_None) \
								 \
	X(1,		"1", 		WM_VInput_Modifier_None) \
	X(2,		"2", 		WM_VInput_Modifier_None) \
	X(3,		"3", 		WM_VInput_Modifier_None) \
	X(4,		"4", 		WM_VInput_Modifier_None) \
	X(5,		"5", 		WM_VInput_Modifier_None) \
	X(6,		"6", 		WM_VInput_Modifier_None) \
	X(7,		"7", 		WM_VInput_Modifier_None) \
	X(8,		"8", 		WM_VInput_Modifier_None) \
	X(9,		"9", 		WM_VInput_Modifier_None) \
	X(0,		"0", 		WM_VInput_Modifier_None) \
								 \
	X(Exclamation,	"!", 		WM_VInput_Modifier_Shift) \
	X(At,		"@", 		WM_VInput_Modifier_Shift) \
	X(Pound,	"#", 		WM_VInput_Modifier_Shift) \
	X(DollarSign,	"$", 		WM_VInput_Modifier_Shift) \
	X(Percent,	"%", 		WM_VInput_Modifier_Shift) \
	X(Carat,	"^", 		WM_VInput_Modifier_Shift) \
	X(And,		"&", 		WM_VInput_Modifier_Shift) \
	X(Star,		"*", 		WM_VInput_Modifier_Shift) \
	X(LeftParen,	"(", 		WM_VInput_Modifier_Shift) \
	X(RightParen,	")", 		WM_VInput_Modifier_Shift) \
								  \
	X(Comma,	",", 		WM_VInput_Modifier_None) \
	X(Period,	".", 		WM_VInput_Modifier_None) \
	X(ForwardSlash,	"/", 		WM_VInput_Modifier_None) \
	X(BackSlash,	"\\", 		WM_VInput_Modifier_None) \
	X(SemiColon,	";", 		WM_VInput_Modifier_None) \
	X(Quote,	"'", 		WM_VInput_Modifier_None) \
	X(BackQuote,	"`", 		WM_VInput_Modifier_None) \
	X(Minus,	"-", 		WM_VInput_Modifier_None) \
	X(Equal,	"=", 		WM_VInput_Modifier_None) \
	X(LeftBracket,	"[", 		WM_VInput_Modifier_None) \
	X(RightBracket,	"]", 		WM_VInput_Modifier_None) \
								 \
	X(LessThan,	"<", 		WM_VInput_Modifier_Shift) \
	X(GreaterThan,	">", 		WM_VInput_Modifier_Shift) \
	X(QuestionMark,	"?", 		WM_VInput_Modifier_Shift) \
	X(Colon,	":", 		WM_VInput_Modifier_Shift) \
	X(DoubleQuote,	"\"", 		WM_VInput_Modifier_Shift) \
	X(Bar,		"|", 		WM_VInput_Modifier_Shift) \
	X(Tilde,	"~", 		WM_VInput_Modifier_Shift) \
	X(Underscore,	"_", 		WM_VInput_Modifier_Shift) \
	X(Plus,		"+", 		WM_VInput_Modifier_Shift) \
	X(LeftBrace,	"{", 		WM_VInput_Modifier_Shift) \
	X(RightBrace,	"}", 		WM_VInput_Modifier_Shift) \


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
#define X(name, str, mod) Glue(WM_VKey_, name),
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
	Vec2F32 mouse_location;
	Vec2F32 mouse_scroll_vector;
	WM_VButton mouse_button;
	WM_VInput_Modifier mouse_mod;
	
	// keybaord related event data 
	WM_VKey key;
	B32 key_is_repeat;
	WM_VInput_Modifier key_mod;

	// window related event data
	Vec2F32 window_location;
	Vec2F32 window_size;

	// System related event data
	OS_Handle system_clip_board;
	OS_Handle system_file_drop;
	F32 system_new_dpi;
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
