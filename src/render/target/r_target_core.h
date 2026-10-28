#ifndef R_TARGET_CORE_H
#define R_TARGET_CORE_H

///////////////////////////////////////////////////////////////////////////////
/// cjk: Render Target Abstraction
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// cjk: window abstraction type
/// 	 this type mirros a a type in the window manager abstraction
/// 	 the void pointer in the r_create_target gets casted to this

typedef U64 R_NativeWindowType;
enum {
	R_NativeWindowType_NONE = 0,
	R_NativeWindowType_X11,
	R_NativeWindowType_Wayland,
	R_NativeWindowType_Win32,
	R_NativeWindowType_Cocoa,
	R_NativeWindowType_COUNT
};

typedef struct {
	R_NativeWindowType type;

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

}R_NativeHandles;



typedef U64 R_TargetBackend;
enum {
	R_TargetBackend_NONE = 0,
	R_TargetBackend_OpenGL,
	R_TargetBackend_Vulkan,
	R_TargetBackend_DX12,
	R_TargetBackend_Metal,
	R_TargetBackend_COUNT
};

typedef struct {
	R_TargetBackend backend;

	union{
		struct{
			void* display;		// EGLDisplay
			void* surface;		// EGLSurface
			void* context;		// EGLContext
			void* config;		// EGLConfig
			void* wl_egl_window; 	// struct wl_egl_window*
			B32 loaded_gl;
		} opengl;

		struct{
			U64 surface;		// VkSurfaceKHR
			U64 instance;		// VkInstance
		} vulkan;
	
		struct {
			void* swapchain;	// IDXGISwapChain* 
		} dx12;

		struct {
			void* metal_layer;	// CAMetalLayer* 
		} metal;
	};

}R_Target;


R_Target* r_create_target(Arena* arena, void* handle);
void r_close_target(R_Target* target);

void r_begin_frame(R_Target* target);
void r_end_frame(R_Target* target);


#endif // R_TARGET_CORE_H 
