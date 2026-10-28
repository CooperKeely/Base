#ifndef R_TARGET_WAYLAND_C
#define R_TARGET_WAYLAND_C

# if defined(ENABLE_RENDER_BACKEND_OPENGL)

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-egl.h>

#define GL_LOADER_IMPLEMENTATION
#include "gl_loader.h"

R_Target* r_create_target(Arena* arena, void* handle)
{
	Assert(handle);

	R_NativeHandles* h = (R_NativeHandles*) handle;
	Assert(h->type == R_NativeWindowType_Wayland);

	struct wl_display* wl_disp = (struct wl_display*) h->wayland.display;
	struct wl_surface* wl_surf = (struct wl_surface*) h->wayland.surface;
	
	Assert(wl_disp);
	Assert(wl_surf);

	EGLint major;
	EGLint minor;
	EGLConfig egl_config;
	EGLint configs_size = 3;
	EGLint num_configs;
	EGLBoolean err;

	EGLint ctx_attributes[] = { 
		EGL_CONTEXT_CLIENT_VERSION, 3,
		EGL_NONE 
	};

	EGLint attributes[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_NONE
	};
	
	EGLDisplay egl_display = eglGetDisplay((EGLNativeDisplayType) wl_disp);
	if(egl_display == EGL_NO_DISPLAY){ LogError("Failed to get EGL display"); }
	else{ LogDebug("Connected to EGL display"); }

	err = eglInitialize(egl_display, &major, &minor); 
	if(!err){ LogError("Failed to initialize EGL: (%d)", eglGetError()); }
	else { LogDebug("Successfully initialized EGL display"); }
	
	err = eglChooseConfig(egl_display, attributes, &egl_config, 1, &num_configs);
	if(!err) { LogError("Failed to choose config: %d", eglGetError()); }
	else { LogDebug("Successfully matched %d configs", num_configs); }

	err = eglBindAPI(EGL_OPENGL_API);
	if(!err) { LogError("Failed to bind to egl OpenGL api"); }
	else { LogDebug("Successfully attached to egl OpenGl Api"); }

	EGLContext egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, ctx_attributes);
	if (egl_context == EGL_NO_CONTEXT){ LogError("Failed to attach to egl context : %d", eglGetError()); }	
	else { LogDebug("Successfully attached to egl context"); }

	struct wl_egl_window* wl_egl_win = wl_egl_window_create(wl_surf, 1, 1);
	if(!wl_egl_win){ LogError("Failed to create a wayland egl window"); }		

	// create egl surface
	EGLSurface egl_surface = eglCreateWindowSurface(egl_display, egl_config, wl_egl_win, NULL);
	if (egl_surface == EGL_NO_SURFACE){ LogError("Failed to attach to native window : %d", eglGetError()); }	
	else { LogDebug("Successfully attached to native window"); }

	R_Target* target = ArenaPushStructZero(arena, R_Target);
	target->backend = R_TargetBackend_OpenGL;
	target->opengl.display = egl_display;
	target->opengl.surface = egl_surface;
	target->opengl.context = egl_context;
	target->opengl.config = egl_config;
	target->opengl.wl_egl_window = wl_egl_win;

	gl_loader_init((GL_LoaderProc) eglGetProcAddress);

	return target;
}


void r_close_target(R_Target* t){
	Assert(t);

	EGLDisplay disp = (EGLDisplay) t->opengl.display;
	EGLSurface surf = (EGLSurface) t->opengl.surface;
	EGLContext ctx = (EGLContext) t->opengl.context;
	struct wl_egl_window* egl_win = (struct wl_egl_window*) t->opengl.wl_egl_window;

	Assert(disp); 
	Assert(surf);
	Assert(ctx);
	Assert(egl_win);

	r_begin_frame(t); // makes the current context the target
	if(surf) 	{ eglDestroySurface(disp, surf); } 	
	if(ctx)		{ eglDestroyContext(disp, ctx); }	
	if(egl_win)	{ wl_egl_window_destroy(egl_win); }	
	eglTerminate(disp);
}

void r_begin_frame(R_Target* t){
	Assert(t);

	EGLDisplay disp = (EGLDisplay) t->opengl.display;
	EGLSurface surf = (EGLSurface) t->opengl.surface;
	EGLContext ctx = (EGLContext) t->opengl.context;
	struct wl_egl_window* egl_win = (struct wl_egl_window*) t->opengl.wl_egl_window;

	Assert(disp); 
	Assert(surf);
	Assert(ctx);
	Assert(egl_win);

	EGLBoolean err = eglMakeCurrent(disp, surf, surf, ctx);
	if (!err) { LogError("Failed to make the current context: %d", eglGetError()); }

	if(!t->opengl.loaded_gl) {
		gl_loader_init((GL_LoaderProc) eglGetProcAddress);
		t->opengl.loaded_gl = BASE_TRUE;
	}


}

void r_end_frame(R_Target* t){
	Assert(t);

	EGLDisplay disp = (EGLDisplay) t->opengl.display;
	EGLSurface surf = (EGLSurface) t->opengl.surface;
	EGLContext ctx = (EGLContext) t->opengl.context;
	struct wl_egl_window* egl_win = (struct wl_egl_window*) t->opengl.wl_egl_window;

	Assert(disp); 
	Assert(surf);
	Assert(ctx);
	Assert(egl_win);
	
	// make the current context
	EGLBoolean err = eglSwapBuffers(disp, surf);
	if (!err) { LogError("Failed to swap egl buffers: %d", eglGetError()); }
}


# elif defined(ENABLE_RENDER_BACKEND_VULKAN)

# endif

#endif // R_TARGET_WAYLAND_C

