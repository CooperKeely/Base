#ifndef RENDERER_H
#define RENDERER_H

#define R_MAX_RENDER_COMMANDS KB(20)

typedef U64 R_RenderingBackend;
enum{
	R_RenderingBackend_Software,
	R_RenderingBackend_Vulkan,
	R_RenderingBackend_OpenGL
};

typedef U64 R_RenderMode;
enum{
	R_RenderMode_3D,
	R_RenderMode_2D
};

typedef U64 R_RenderCommandType;
enum{
	R_RenderCommandType_Line,
	R_RenderCommandType_Triangle,
	R_RenderCommandType_Quad
};

typedef U64 R_RenderCommandOption;
enum{
	R_RenderCommandOption_None,
	R_RenderCommandOption_Wireframe,
	R_RenderCommandOption_Filled,
};

typedef U64 R_RenderProjection;
enum{
	R_RenderProjection_Perspective,
	R_RenderProjection_Orthographic,
};

typedef struct {
	R_RenderProjection projection;
	R_RenderMode mode;
	Vec3F32 position;
	Vec3F32 target;
	Vec3F32 up_dir;
	F32 fov;
}R_RenderCamera;

typedef struct{
	R_RenderCommandType type;
	R_RenderCommandOption options;
	ColorRGBA color;
	union{
		struct {Vec3F32 p0, p1; } line;
		struct {Vec3F32 p0, p1, p2 ; } triangle;
		struct {Vec3F32 p0, p1, p2, p3; } quad;
	};
} R_RenderCommand;

typedef struct{
	U32 width;
	U32 height;
	U32 stride;
	void* buffer;		// used for software renering
	void* os_handle;	// used for vulkan/opengl rendering
} R_RenderTarget;

// forward declare R_RenderContext;
typedef struct R_RenderContext R_RenderContext;

typedef struct{
	void (*r_init_backend)(R_RenderContext* ctx);	
	void (*r_begin_frame)(R_RenderContext *ctx, R_RenderTarget target);	
	void (*r_submit_commands)(R_RenderCommand *cmd, U64 count);	
	void (*r_end_frame)(R_RenderContext *ctx);	
	void (*r_close_backend)(R_RenderContext *ctx);	
}R_BackendInterface;


struct R_RenderContext{
	R_RenderingBackend backend;	
	R_BackendInterface api;	

	R_RenderCamera camera;	

	R_RenderCommand* command_queue;
	U64 capacity;
	U64 count; 
	
	R_RenderTarget target;

	void* backend_data;
};

global R_RenderContext* glb_r_context = NULL;

void r_set_global_render_context(R_RenderContext* ctx);
R_RenderContext* r_get_global_render_context();

R_RenderCommand* r_push_render_command(R_RenderCommandType type);

// api wrapper functions
R_RenderContext* r_init_render(Arena* arena, R_RenderingBackend backend);
void r_close_render();
void r_begin_frame(R_RenderTarget target);
void r_end_frame();

// basic primitive drawing
void r_draw_line(Vec3F32 p0, Vec3F32 p1, ColorRGBA c);
void r_draw_triangle(Vec3F32 p0, Vec3F32 p1, Vec3F32 p2, ColorRGBA c);
void r_draw_quad(Vec3F32 p0, Vec3F32 p1, Vec3F32 p2, Vec3F32 p3, ColorRGBA c);

#endif //RENDERER_H
