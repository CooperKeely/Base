#ifndef OS_LINUX_WAYLAND_GFX_H
#define OS_LINUX_WAYLAND_GFX_H

// wayland includes
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "../../../include/xdg-shell-client-protocol.h"

// opengl function loader
#define GL_LOADER_IMPLEMENTATION
#include "../../libs/gl_loader.h"

static LOG_Context log_way = (LOG_Context){
	.fd 		= LOG_FD_STDERR,
	.level 		= LOG_Level_All,
	.log_flags	= LOG_Option_TimeStamp | LOG_Option_Abort,
	.category	= { sizeof("WAYLAND") - 1, "WAYLAND" },
};

struct WM_PlatformContext{
	// globals
	struct wl_display* wl_display; 
	struct wl_registry* wl_registry;
	struct wl_compositor* wl_compositor;
	struct xdg_wm_base* xdg_wm_base;

	// objects
	struct wl_surface* wl_surface;
	struct xdg_surface* xdg_surface;
	struct xdg_toplevel* xdg_toplevel;

	// egl context
	EGLDisplay egl_display;
	EGLContext egl_context;
	EGLSurface egl_surface;
	struct wl_egl_window* egl_window;
};

void wm_init_egl(WM_PlatformContext* ctx, U32 width, U32 height);

// (cjk): Register Listeners

static void registry_handle_global(void* data, struct wl_registry *registry, U32 name, const char* interface, U32 version);
static void registry_handle_global_remove(void* data, struct wl_registry* registry, U32 name);
static void xdg_surface_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial);
static void xdg_wm_base_ping(void* data, struct xdg_wm_base *xdg_wm_base, uint32_t serial);


static const struct wl_registry_listener registry_listener = {
	.global = registry_handle_global,
	.global_remove = registry_handle_global_remove,
};

static const struct xdg_surface_listener xdg_surface_listener = {
	.configure = xdg_surface_configure,
};

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
	.ping = xdg_wm_base_ping,
};

#endif //OS_LINUX_WAYLAND_H
