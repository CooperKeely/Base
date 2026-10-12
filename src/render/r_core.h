
typedef U64 R_ConfigFlag;
enum{
	R_ConfigFlag_None,
	R_ConfigFlag_COUNT
};

typedef struct {
	U64 u64[1];
} R_Handle;

typedef struct {
	R_ConfigFlag flags;	
} R_Context;

R_Context* r_init(Arena* arena, R_ConfigFlag flags);
void r_close(R_Context* ctx);

R_Handle r_attach_to_window(R_Context* ctx, WM_Window window, U32 w, U32 h);
void r_detach_from_window(R_Context* ctx, WM_Window window);

void r_begin_frame(R_Context* ctx);
void r_end_frame(R_Context* ctx);

void r_submit_frame(R_Context* ctx);
