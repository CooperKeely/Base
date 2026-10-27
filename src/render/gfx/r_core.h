
typedef U64 R_ConfigFlag;
enum{
	R_ConfigFlag_None,
	R_ConfigFlag_COUNT
};

typedef struct Handle R_Target;
typedef struct Handle R_Context;

R_Context r_init(Arena* arena, R_ConfigFlag flags);
void r_close(R_Context ctx);

void r_start_pass(R_Context ctx, GFX_Surface* surface);
void r_end_pass(R_Context ctx);

