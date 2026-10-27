#ifndef OS_LINUX_WAYLAND_GFX_H
#define OS_LINUX_WAYLAND_GFX_H

// wayland includes
#undef global

#include <wayland-client.h>
#include <wayland-egl.h>
#include "../../../include/xdg-shell-client-protocol.h"

#define global static

static LOG_Context log_way = (LOG_Context){
	.fd 		= LOG_FD_STDERR,
	.level 		= LOG_Level_All,
	.log_flags	= LOG_Option_TimeStamp | LOG_Option_Abort,
	.category	= { sizeof("WAYLAND") - 1, (U8*)"WAYLAND" },
};


///////////////////////////////////////////////////////////////////////////////
/// cjk: Window Manager Wayland Specific Data Types

typedef struct WM_WaylandDisplay WM_WaylandDisplay;
typedef struct WM_WaylandWindow WM_WaylandWindow;
typedef struct WM_WaylandMonitor WM_WaylandMonitor;


struct WM_WaylandDisplay {
	
	// doubly linked list of all made windows
	WM_WaylandWindow* window_head;
	WM_WaylandWindow* window_tail;

	// doubly linked list of all made monitors
	WM_WaylandMonitor* monitor_head;
	WM_WaylandMonitor* monitor_tail;

	// globals
	struct wl_display* wl_display; 
	struct wl_registry* wl_registry;
	struct wl_compositor* wl_compositor;
	struct xdg_wm_base* xdg_wm_base;
};

struct WM_WaylandWindow{
	// Doubly linked list of all windows
	WM_WaylandWindow* next;
	WM_WaylandWindow* prev;

	WM_WaylandMonitor* parent_monitor;
	
	// Persistent internal event queue
	Arena* current_frame_arena; // this is used to allocate
	WM_Event* event_head;
	WM_Event* event_tail;

	// objects
	struct wl_surface* wl_surface;
	struct xdg_surface* xdg_surface;
	struct xdg_toplevel* xdg_toplevel;

	Str8 name;

	F32 width;
	F32 height;

	B8 is_focused;
	B8 is_maximized;
	B8 fullscreen;
}; 


struct WM_WaylandMonitor{
	// doubly linked list of monitors
	WM_WaylandMonitor* next;
	WM_WaylandMonitor* prev;

	// doubly linked list of all made windows
	WM_WaylandWindow* head;
	WM_WaylandWindow* tail;
	
	// data
	Pnt2U32 dimensions;
	Pnt2U32 resolution;
	F32 	dpi;
	B32 	hi_dpi;
	F32 	hz;
	Str8 	name;
};

global WM_WaylandDisplay g_wm_wayland_display = {0};

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland Display Discovery

static void registry_handle_global(void* data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version);
static void registry_handle_global_remove(void* data, struct wl_registry* registry, uint32_t name);

static const struct wl_registry_listener g_registry_listener = {
	registry_handle_global,
	registry_handle_global_remove,
};


///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland XDG Window Management

// xdg_wm_base
static void xdg_wm_base_handle_ping(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial);

static const struct xdg_wm_base_listener g_xdg_wm_base_listener = {
    .ping = xdg_wm_base_handle_ping,
};

// xdg_surface
static void xdg_surface_handle_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial);

static const struct xdg_surface_listener g_xdg_surface_listener = {
    .configure = xdg_surface_handle_configure,
};

// xdg_toplevel
static void xdg_toplevel_handle_configure(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height, struct wl_array* states);
static void xdg_toplevel_handle_close(void* data, struct xdg_toplevel* xdg_toplevel);
static void xdg_toplevel_handle_configure_bounds(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height);
static void xdg_toplevel_handle_wm_capabilities(void* data, struct xdg_toplevel* xdg_toplevel, struct wl_array* capabilities);

static const struct xdg_toplevel_listener g_xdg_toplevel_listener = {
    .configure        = xdg_toplevel_handle_configure,
    .close            = xdg_toplevel_handle_close,
    .configure_bounds = xdg_toplevel_handle_configure_bounds,
    .wm_capabilities  = xdg_toplevel_handle_wm_capabilities,
};

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland Keyboard and Mouse Listeners

// wl_seat
static void seat_handle_capabilities(void* data, struct wl_seat* seat, uint32_t caps);
static void seat_handle_name(void* data, struct wl_seat* seat, const char* name);

static const struct wl_seat_listener g_seat_listener = {
    .capabilities = seat_handle_capabilities,
    .name         = seat_handle_name,
};

// wl_keyboard
static void keyboard_handle_keymap(void* data, struct wl_keyboard* kb, uint32_t format, int32_t fd, uint32_t size);
static void keyboard_handle_enter(void* data, struct wl_keyboard* kb, uint32_t serial, struct wl_surface* surface, struct wl_array* keys);
static void keyboard_handle_leave(void* data, struct wl_keyboard* kb, uint32_t serial, struct wl_surface* surface);
static void keyboard_handle_key(void* data, struct wl_keyboard* kb, uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
static void keyboard_handle_modifiers(void* data, struct wl_keyboard* kb, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group);
static void keyboard_handle_repeat_info(void* data, struct wl_keyboard* kb, int32_t rate, int32_t delay);

static const struct wl_keyboard_listener g_keyboard_listener = {
    .keymap      = keyboard_handle_keymap,
    .enter       = keyboard_handle_enter,
    .leave       = keyboard_handle_leave,
    .key         = keyboard_handle_key,
    .modifiers   = keyboard_handle_modifiers,
    .repeat_info = keyboard_handle_repeat_info,
};

// wl_pointer
static void pointer_handle_enter(void* data, struct wl_pointer* ptr, uint32_t serial, struct wl_surface* surface, wl_fixed_t sx, wl_fixed_t sy);
static void pointer_handle_leave(void* data, struct wl_pointer* ptr, uint32_t serial, struct wl_surface* surface);
static void pointer_handle_motion(void* data, struct wl_pointer* ptr, uint32_t time, wl_fixed_t sx, wl_fixed_t sy);
static void pointer_handle_button(void* data, struct wl_pointer* ptr, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
static void pointer_handle_axis(void* data, struct wl_pointer* ptr, uint32_t time, uint32_t axis, wl_fixed_t value);
static void pointer_handle_frame(void* data, struct wl_pointer* ptr);
static void pointer_handle_axis_source(void* data, struct wl_pointer* ptr, uint32_t axis_source);
static void pointer_handle_axis_stop(void* data, struct wl_pointer* ptr, uint32_t time, uint32_t axis);
static void pointer_handle_axis_discrete(void* data, struct wl_pointer* ptr, uint32_t axis, int32_t discrete);

static const struct wl_pointer_listener g_pointer_listener = {
    .enter         = pointer_handle_enter,
    .leave         = pointer_handle_leave,
    .motion        = pointer_handle_motion,
    .button        = pointer_handle_button,
    .axis          = pointer_handle_axis,
    .frame         = pointer_handle_frame,
    .axis_source   = pointer_handle_axis_source,
    .axis_stop     = pointer_handle_axis_stop,
    .axis_discrete = pointer_handle_axis_discrete,
};

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland Rendering & Sync listeners

// wl_callback (Frame Pacing / VSync)
static void frame_handle_done(void* data, struct wl_callback* callback, uint32_t callback_data);

static const struct wl_callback_listener g_frame_listener = {
    .done = frame_handle_done,
};

// wl_buffer (Software Rendering / Release notification)
static void buffer_handle_release(void* data, struct wl_buffer* buffer);

static const struct wl_buffer_listener g_buffer_listener = {
    .release = buffer_handle_release,
};

///////////////////////////////////////////////////////////////////////////////
/// cjk: Wayland Monitor Listeners

static void output_handle_geometry(void* data, struct wl_output* output, int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const char* make, const char* model, int32_t transform);
static void output_handle_mode(void* data, struct wl_output* output, uint32_t flags, int32_t width, int32_t height, int32_t refresh);
static void output_handle_done(void* data, struct wl_output* output);
static void output_handle_scale(void* data, struct wl_output* output, int32_t factor);
static void output_handle_name(void* data, struct wl_output* output, const char* name);
static void output_handle_description(void* data, struct wl_output* output, const char* description);

static const struct wl_output_listener g_output_listener = {
    .geometry    = output_handle_geometry,
    .mode        = output_handle_mode,
    .done        = output_handle_done,
    .scale       = output_handle_scale,
    .name        = output_handle_name,
    .description = output_handle_description,
};
#endif //OS_LINUX_WAYLAND_H
