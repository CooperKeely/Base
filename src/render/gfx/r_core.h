#ifndef R_CORE_H
#define R_CORE_H

typedef U64 R_ConfigFlag;
enum{
	R_ConfigFlag_None,
	R_ConfigFlag_COUNT
};

typedef struct Handle R_Context;

R_Context r_create_context(Arena* arena, R_ConfigFlag flags);
void r_close_context(R_Context ctx);

void r_start_pass(R_Context ctx, R_Target* target);
void r_end_pass(R_Context ctx);

#endif // R_CORE_H
