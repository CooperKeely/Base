#ifndef BASE_OS_GFX_H
#define BASE_OS_GFX_H

///////////////////////////////////////
/// cjk: Window API Definitions 

typedef U64 OS_GFX_EventFlag;

typedef struct OS_GFX_WindowContext OS_GFX_WindowContext;

// Open and closing window
OS_GFX_WindowContext* os_gfx_open_window(Arena* arena, RectU16 win_rect, Str8 window_name, U16 border_width, ColorRGBA border_color,  ColorRGBA background_color);
void os_gfx_close_window(OS_GFX_WindowContext* ctx);

// Resizing the window
void os_gfx_resize_window(OS_GFX_WindowContext* ctx, U16 width, U16 height);
void os_gfx_move_window(OS_GFX_WindowContext* ctx, U16 x, U16 y);
void os_gfx_resize_and_move_window(OS_GFX_WindowContext* ctx, RectU16 new_size);
void os_gfx_draw_window(OS_GFX_WindowContext* ctx);

// Event handeling
void os_gfx_register_input_events(OS_GFX_WindowContext* ctx, OS_GFX_EventFlag flags);
U32 os_gfx_num_of_pending_events(OS_GFX_WindowContext* ctx);



#endif // BASE_OS_GFX
