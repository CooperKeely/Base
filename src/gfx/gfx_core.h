#ifndef GFX_CORE_H
#define GFX_CORE_H

///////////////////////////////////////////////////////////////////////////////
/// cjk: Window Manager Graphics Backend Abstraction

typedef U64 GFX_BackendType;
enum {
	GFX_BackendType_NONE = 0,
	GFX_BackendType_OpenGL,
	GFX_BackendType_Vulkan,
	GFX_BackendType_DX12,
	GFX_BackendType_Metal,
	GFX_Backend_COUNT
};

typedef struct {
	GFX_BackendType backend;
	
	union{
		struct{
			void* display;
			void* surface;
			void* context;
			void* wl_egl_window; //wayland only
		} egl;

		struct{
			U64 surface;
		} vk;
	
		struct {
			U64 id; // placeholder
		} dx12;

		struct {
			U64 id // placeholder
		} metal;
	};

}GFX_Surface;


GFX_Surface gfx_init(GFX_BackendType backend,);


#endif // GFX_CORE_H 
