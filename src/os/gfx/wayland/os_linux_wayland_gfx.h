#ifndef OS_LINUX_WAYLAND_GFX_H
#define OS_LINUX_WAYLAND_GFX_H

// wayland includes
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "../../../../include/xdg-shell-client-protocol.h"

// opengl function loader
#define GL_LOADER_IMPLEMENTATION
#include "../../../libs/gl_loader.h"

struct OS_GFX_PlatformContext{
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

void os_gfx_init_egl(OS_GFX_PlatformContext* ctx, U32 width, U32 height);

#endif //OS_LINUX_WAYLAND_H
