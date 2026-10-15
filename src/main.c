///////////////////////////////////////
/// cjk: TODO:
/// [ ]: Add Arena Free List Implementation
/// [ ]: Finish Window Mangager API
/// 	- Add event list
/// 	- depends on Arena free list
/// [ ]: Finish Render Hardware Inferface API
/// 	- https://www.youtube.com/watch?v=AvVgYcvaY0c 
/// [ ]: Add Font Rendering
/// 	- ttf file parsing
/// [ ]: Add UI toolkit
/// 	- Clay : https://github.com/nicbarker/clay
/// 	- DearIMGUI : https://github.com/ocornut/imgui
/// 	- https://www.youtube.com/watch?v=7Tr0ty9-yeQ&t=3974s


///////////////////////////////////////
/// cjk: header files include


#include "base/base_inc.h"
#include "wm/wm_inc.h"

#define RENDER_ENABLE_OPENGL
#include "render/r_inc.h"

S32 entry_point(U64 argc, U8** argv){
	(void) argc;
	(void) argv;
	
	Arena* app_arena = arena_alloc();
	Arena* frame_arena = arena_alloc();

	WM_Display display = wm_init(app_arena, WM_ConfigFlag_Resizeable);
	WM_Window window = wm_window_create(display, app_arena, 1000, 1000, Str8Lit("Software Renderer"));

	// Initialize Rendering Context
	R_Context render_context = r_init(app_arena, R_ConfigFlag_None);
	R_Target render_target = r_attach_to_target(render_context, window, 1000, 1000);	

	B32 running = BASE_TRUE;
	while(running){
		// reads the OS events into a doubly linked list using
		WM_Event* event_head = wm_event_get_list(window, frame_arena);

		// traverse the doubly linked list
		for EachNode(e, WM_Event, event_head){
			switch(e->type){
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
			case WM_EventType_System_Quit:{
				running = BASE_FALSE;
			}break;
			default:{}break;
			}	

		}

		r_begin_frame(render_target);
		// rendering api must happen here
		r_end_frame(render_target);
		r_submit_frame(render_target);

		// de allocate all the frame related data
		arena_pop_to(frame_arena, 0);
	}

	r_close(render_context);
	wm_close(display);

	arena_release(app_arena);
	arena_release(frame_arena);

	return 0;
}


int main(int argc, char** argv){os_entry_point(argc, (U8**) argv, &entry_point);}
