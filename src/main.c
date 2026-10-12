///////////////////////////////////////
/// cjk: header files include


#include "base/base_inc.h"
#include "wm/wm_inc.h"

#define RENDER_ENABLE_OPENGL
#include "render/r_inc.h"


internal B32 frame(void* data){

}

S32 entry_point(U64 argc, U8** argv){
	(void) argc;
	(void) argv;
	
	Arena* arena = arena_alloc();

	// Open the window
	// give the reference to the context when the user does
	// wm_init();
	// then on creation of the window we can pass the WM_Backend handle
	// so they can easily just pass that to the rendering backend


	WM_Context* wm_ctx = wm_init(arena, WM_ConfigFlag_Resizeable);
	WM_Window window = wm_create_window(wm_ctx, 1000, 1000, Str8Lit("Software Renderer"));

	// Initialize Rendering Context
	R_Context* r_ctx = r_init(arena, R_ConfigFlag_None);
	R_Handle r_handle = r_attach_to_window(r_ctx, window, 1000, 1000);	

	B32 running = BASE_TRUE;
	while(running){
		WM_Event event;
		while(wm_get_events(wm_ctx, &event)){
			switch(event.type){
				// Window related events
				case WM_EventType_Window_Shown:{}break;
				case WM_EventType_Window_Hidden:{}break;
				case WM_EventType_Window_Redraw:{}break;
				case WM_EventType_Window_FocusGained:{}break;
				case WM_EventType_Window_FocusLost:{}break;
				case WM_EventType_Window_Close:{}break;
				case WM_EventType_Window_Resize:{}break;
				case WM_EventType_Window_Move:{}break;

				// Keyboard related events
				case WM_EventType_Key_Press:{}break;
				case WM_EventType_Key_Release:{}break;
				case WM_EventType_Key_MapChange:{}break;

				// Mouse related events
				case WM_EventType_Mouse_EnterWindow:{}break;
				case WM_EventType_Mouse_LeaveWindow:{}break;
				case WM_EventType_Mouse_Motion:{}break;
				case WM_EventType_Mouse_Scroll:{}break;
				case WM_EventType_Mouse_Press:{}break;
				case WM_EventType_Mouse_Release:{}break;

				// System updates
				case WM_EventType_System_ClipboardUpdate:{}break;
				case WM_EventType_System_DPIChange:{}break;
				case WM_EventType_System_DropFile:{}break;
				case WM_EventType_System_Quit:{}break;
				default:{}break;
			}	

		}

		r_begin_frame(r_ctx);

		r_end_frame(r_ctx);
		r_submit_frame(r_ctx);
	}

	r_close(r_ctx);
	wm_close(wm_ctx);

	arena_release(arena);

	return 0;
}


int main(int argc, char** argv){os_entry_point(argc, (U8**) argv, &entry_point);}
