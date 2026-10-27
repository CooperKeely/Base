
R_Context r_init(Arena* arena, R_ConfigFlag flags){}
void r_close(R_Context ctx){}

R_Target r_attach_to_target(R_Context ctx, WM_Window window, U32 w, U32 h){}
void r_detach_from_target(R_Target target){}

void r_start_pass(R_Target target){}
void r_end_pass(R_Target target){}

