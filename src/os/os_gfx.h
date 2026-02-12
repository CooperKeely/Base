#ifndef BASE_OS_GFX_H
#define BASE_OS_GFX_H

///////////////////////////////////////
/// cjk: Window API Definitions 

#ifdef BASE_ENABLE_WINDOW

# include <math.h>

# include <xcb/xcb.h>
# include <xcb/shm.h>
# include <sys/ipc.h>
# include <sys/shm.h>


typedef U32 WM_WindowCfgUpdate;
enum{
	WM_WindowCfgUpdate_X		= (1<<0),
	WM_WindowCfgUpdate_Y		= (1<<1),
	WM_WindowCfgUpdate_Width	= (1<<2),
	WM_WindowCfgUpdate_Height	= (1<<3),
	WM_WindowCfgUpdate_BorderWidth	= (1<<4),
	WM_WindowCfgUpdate_Sibiling	= (1<<5),
	WM_WindowCfgUpdate_StackMode	= (1<<6),
};


typedef U64 WM_EventFlag;
enum{
	WM_Event_Keyboard_KeyPress		= (1ULL<<0),
	WM_Event_Keyboard_KeyRelease		= (1ULL<<1),
	WM_Event_Mouse_ButtonPress		= (1ULL<<2),
	WM_Event_Mouse_ButtonRelease		= (1ULL<<3),
	WM_Event_Mouse_MotionNotify		= (1ULL<<4),
	WM_Event_Window_EnterNotify		= (1ULL<<5),
	WM_Event_Window_LeaveNotify		= (1ULL<<6),
	WM_Event_Focus_FocusIn			= (1ULL<<7),
	WM_Event_Focus_FocusOut			= (1ULL<<8),
	WM_Event_KeyMap_KeymapNotify		= (1ULL<<9),
	WM_Event_Exposure_Expose		= (1ULL<<10),
	WM_Event_Exposure_GraphicsExpose	= (1ULL<<11),
	WM_Event_Exposure_NoExpose		= (1ULL<<12),
	WM_Event_Structure_CirculateRequest	= (1ULL<<13),
	WM_Event_Strucutre_ConfigureRequest	= (1ULL<<14),
	WM_Event_Structure_MapRequest		= (1ULL<<15),
	WM_Event_Structure_ResizeRequest	= (1ULL<<16),
	WM_Event_WindowState_CirculateNotify	= (1ULL<<17),
	WM_Event_WindowState_ConfigureNotify	= (1ULL<<18),
	WM_Event_WindowState_CreateNotify	= (1ULL<<19),
	WM_Event_WindowState_DestroyNotify	= (1ULL<<20),
	WM_Event_WindowState_GravityNotif	= (1ULL<<21),
	WM_Event_WindowState_MapNotify		= (1ULL<<22),
	WM_Event_WindowState_MappingNotify	= (1ULL<<23),
	WM_Event_WindowState_ReparentNotify	= (1ULL<<24),
	WM_Event_WindowState_UnmapNotify	= (1ULL<<25),
	WM_Event_WindowState_VisibilityNotify	= (1ULL<<26),
	WM_Event_Colormap_ColormapNotify	= (1ULL<<27),
	WM_Event_ClientComm_ClientMessage	= (1ULL<<28),
	WM_Event_ClientComm_PropertyNotify	= (1ULL<<29),
	WM_Event_ClientComm_SelectionClear	= (1ULL<<30),
	WM_Event_ClientComm_SelectionNotify	= (1ULL<<31),
	WM_Event_ClientComm_SelectionRequest	= (1ULL<<32),
};

// Maps our window manager events to x11 event request
typedef struct{
	WM_EventFlag flag;	
	long event_mask;
}WM_EventMaskMap;

global WM_EventMaskMap wm_event_mask_map[] = {
	{WM_Event_Keyboard_KeyPress		,KeyPressMask},
	{WM_Event_Keyboard_KeyRelease		,KeyReleaseMask},
	{WM_Event_Mouse_ButtonPress		,ButtonPressMask},
	{WM_Event_Mouse_ButtonRelease		,ButtonReleaseMask},
	{WM_Event_Mouse_MotionNotify		,PointerMotionMask},
	{WM_Event_Window_EnterNotify		,EnterWindowMask},
	{WM_Event_Window_LeaveNotify		,LeaveWindowMask},
	{WM_Event_Focus_FocusIn			,FocusChangeMask},
	{WM_Event_Focus_FocusOut		,FocusChangeMask},
	{WM_Event_KeyMap_KeymapNotify		,KeymapStateMask},
	{WM_Event_Exposure_Expose		,ExposureMask},
	{WM_Event_Exposure_GraphicsExpose	,GCGraphicsExposures},
	{WM_Event_Exposure_NoExpose		,GCGraphicsExposures},
	{WM_Event_Structure_CirculateRequest	,SubstructureRedirectMask},
	{WM_Event_Strucutre_ConfigureRequest	,SubstructureRedirectMask},
	{WM_Event_Structure_MapRequest		,SubstructureRedirectMask},
	{WM_Event_Structure_ResizeRequest	,ResizeRedirectMask},
	{WM_Event_WindowState_CirculateNotify	,StructureNotifyMask},
	{WM_Event_WindowState_ConfigureNotify	,StructureNotifyMask},
	{WM_Event_WindowState_CreateNotify	,SubstructureNotifyMask},
	{WM_Event_WindowState_DestroyNotify	,StructureNotifyMask},
	{WM_Event_WindowState_GravityNotif	,StructureNotifyMask},
	{WM_Event_WindowState_MapNotify		,StructureNotifyMask},
	{WM_Event_WindowState_MappingNotify	,NoEventMask},
	{WM_Event_WindowState_ReparentNotify	,StructureNotifyMask},
	{WM_Event_WindowState_UnmapNotify	,StructureNotifyMask},
	{WM_Event_WindowState_VisibilityNotify	,VisibilityChangeMask},
	{WM_Event_Colormap_ColormapNotify	,ColormapChangeMask},
	{WM_Event_ClientComm_ClientMessage	,NoEventMask},
	{WM_Event_ClientComm_PropertyNotify	,PropertyChangeMask},
	{WM_Event_ClientComm_SelectionClear	,NoEventMask},
	{WM_Event_ClientComm_SelectionNotify	,NoEventMask},
	{WM_Event_ClientComm_SelectionRequest	,NoEventMask},
};

typedef struct{
	xcb_connection_t*	connection;
	xcb_screen_t*		screen;
	xcb_window_t		window;	
	xcb_setup_t* 		setup;
	xcb_gcontext_t		graphics_ctx;
	xcb_shm_seg_t		shared_mem_seg;
	U64			shared_mem_seg_id;
	Arena*			arena;
	RectU16			size;
	RectU16			over_size;
	Str8			name;
}WM_Context;


// Open and closing window
WM_Context wm_open_window(Arena* arena, RectU16 win_rect, Str8 window_name, U16 border_width, ColorRGBA border_color,  ColorRGBA background_color);
void wm_close_window(WM_Context* ctx);

// Resizing the window
void wm_resize_window(WM_Context* ctx, U16 width, U16 height);
void wm_move_window(WM_Context* ctx, U16 x, U16 y);
void wm_resize_and_move_window(WM_Context* ctx, RectU16 new_size);
void wm_draw_window(WM_Context* ctx);

// 2d primitive drawing
void wm_draw_rect(WM_Context* ctx, RectF32 rect, ColorRGBA color);
void wm_draw_circle(WM_Context* ctx, Vec2F32 center, F32 radius, ColorRGBA color) ;
void wm_draw_line(WM_Context* ctx, Vec2F32 p1, Vec2F32 p2, ColorRGBA color);
void wm_draw_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color) ;
B32 wm_is_point_in_triangle(Vec2F32 point, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3);
void wm_draw_filled_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color);

// Event handeling
void wm_register_input_events(WM_Context* ctx, WM_EventFlag flags);
U32 wm_num_of_pending_events(WM_Context* ctx);



#endif // BASE_OS_GFX
