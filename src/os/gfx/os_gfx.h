#ifndef BASE_OS_GFX_H
#define BASE_OS_GFX_H


///////////////////////////////////////
/// cjk: Window API Definitions 

typedef U64 OS_GFX_WindowConfigFlag;
enum{
	OS_GFX_WindowConfigFlag_None		= 0,
	OS_GFX_WindowConfigFlag_Fullscreen 	= (1<<0),
	OS_GFX_WindowConfigFlag_Resizeable	= (1<<1),
	OS_GFX_WindowConfigFlag_Undecorated	= (1<<2),
	OS_GFX_WindowConfigFlag_Hidden		= (1<<3),
	OS_GFX_WindowConfigFlag_Minimized	= (1<<4),
	OS_GFX_WindowConfigFlag_Maximized	= (1<<5),
	OS_GFX_WindowConfigFlag_Unfocused	= (1<<6),
	OS_GFX_WindowConfigFlag_TopMost		= (1<<7),
	OS_GFX_WindowConfigFlag_AlwaysRun	= (1<<8),
	OS_GFX_WindowConfigFlag_COUNT
};

typedef enum{
	OS_GFX_PixelFormat_None = 0,	
	OS_GFX_PixelFormat_RGB_24,
	OS_GFX_PizelFormat_BGR_24,
	OS_GFX_PixelFormat_RGBA_32,
	OS_GFX_PixelFormat_BGRA_32,
	OS_GFX_PixelFormat_BGR_32,
	OS_GFX_PixelFormat_COUNT
}OS_GFX_PixelFormat;


typedef struct{
	F32 double_click_time;
	F32 default_refresh_rate; 
	F32 
}OS_GFX_ConfigValues;

typedef OS_Handle OS_Window;

typedef struct {
	Str8 title;
	OS_GFX_WindowConfigFlag flags;
	OS_GFX_PizelFormat pixel_format;

	Pnt2U32 display_size;
	Pnt2U32 screen_size;
	Pnt2U32 position;
	Pnt2U32 previous_screen_size;
	Pnt2U32 previous_position;

	Pnt2U32 screen_size_min;
	Pnt2U32 screen_size_max;

	OS_Window window;	
}OS_GFX_Context;


global OS_GFX_Context* glb_os_gfx_context;

// Open and closing window
OS_GFX_Context* os_gfx_init_window(Arena* arena, U32 x, U32 y, U32 width, U32 height, Str8 window_name);
void os_gfx_close_window(void);
void os_gfx_init_platform(Arena* arena);
void os_gfx_close_platform(void);
void os_gfx_set_global_context(OS_GFX_Context* ctx);	
OS_GFX_Context* os_gfx_get_current_context();

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

// 
void os_gfx_swap_screen_buffer(void);                      // Swap back buffer with front buffer (screen drawing)
void os_gfx_poll_input_events(void);                       // Register all input events
void* os_gfx_get_current_frame_buffer(void);
void os_gfx_reset_frame_buffers(void);


///////////////////////////////////////
/// cjk: Input Handeling API Definitions 

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
	OS_VInput_Modifier_None 		= 0,
	OS_VInput_Modifier_Shift 		= (1<<0),
	OS_VInput_Modifier_Function 		= (1<<1),
	OS_VInput_Modifier_Control 		= (1<<2),
	OS_VInput_Modifier_Super 		= (1<<3),
	OS_VInput_Modifier_Alt 			= (1<<4),
	OS_VInput_Modifier_CapsLock 		= (1<<5),
	OS_VInput_Modifier_COUNT
}OS_VInput_Modifier;


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


// Input-related functions: mouse 

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


///////////////////////////////////////
/// cjk: OS_Event api 

typedef enum{
	OS_EventType_None,
	
	// Window related events
	OS_EventType_Window_Shown,
	OS_EventType_Window_Hidden,
	OS_EventType_Window_Redraw,
	OS_EventType_Window_FocusGained,
	OS_EventType_Window_FocusLost,
	OS_EventType_Window_Close,
	OS_EventType_Window_Resize,
	OS_EventType_Window_Move,

	// Keyboard related events
	OS_EventType_Key_Press,
	OS_EventType_Key_Release,
	OS_EventType_Key_MapChange,
	
	// Mouse related events
	OS_EventType_Mouse_EnterWindow,
	OS_EventType_Mouse_LeaveWindow,
	OS_EventType_Mouse_Motion,
	OS_EventType_Mouse_Scroll,
	OS_EventType_Mouse_Press,
	OS_EventType_Mouse_Release,

	// System updates
	OS_EventType_System_ClipboardUpdate,
	OS_EventType_System_DPIChange,
	OS_EventType_System_DropFile,
	OS_EventType_System_Quit,

	OS_EventType_COUNT
} OS_EventType;

typedef struct{
	Pnt2U32 location;
	Vec2F32 scroll_vector;
	OS_VButton button;
	OS_VInput_Modifier mod;
} OS_Event_Mouse;

typedef struct{
	OS_VKey key;
	B32 is_repeat;
	OS_VInput_Modifier mod;
} OS_Event_Key;

typedef union{
	Pnt2U32 location; 		
	Rnt2U32 size;
} OS_Event_Window;

typedef union{
	OS_Handle clip_board;
	OS_Handle file_drop;
	U64 new_dpi;
} OS_Event_System;

typedef struct{
	OS_EventType type;
	OS_Window window;
	U64 time_stamp;	

	union{
		OS_Event_Key key_event;
		OS_Event_Mouse mouse_event;
		OS_Event_Window window_event;
		OS_Event_System system_event;
	};
}OS_Event;

B32 os_gfx_poll_input_events(OS_Event* event);

#endif // BASE_OS_GFX
