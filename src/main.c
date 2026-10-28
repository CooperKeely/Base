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
/// cjk: Test Main


#define ENABLE_RENDER_BACKEND_OPENGL

#include "base/base_inc.h"
#include "wm/wm_inc.h"
#include "render/r_inc.h"

S32 single_window(U64 argc, U8** argv){
	(void) argc;
	(void) argv;
	
	Arena* app_arena = arena_alloc();

	wm_create_connection(WM_ConfigFlag_None);
	
	// Viewport 
	WM_Window viewport = wm_create_window(app_arena, 1000, 1000, Str8Lit("Viewport"));
	R_Target* viewport_render_target = r_create_target(app_arena, wm_get_native_handles(app_arena, viewport));
	
	// Shared Rendering Context
	R_Context render_context = r_create_context(app_arena, R_ConfigFlag_None);

	B32 viewport_open = BASE_TRUE;
	Arena* frame_arena = arena_alloc();

	while(viewport_open){

		// reads the OS events into a doubly linked list
		WM_Event* viewport_events = wm_get_event_list(viewport, frame_arena);

		// traverse the doubly linked list
		for EachNode(e, WM_Event, viewport_events){
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
				viewport_open = BASE_FALSE;
			}break;
			default:{}break;
			}	
		}

	
		if(viewport_open){
			r_begin_frame(viewport_render_target);
			{
				r_start_pass(render_context, viewport_render_target);

				//rendering api must happen here
				glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);


				r_end_pass(render_context);
			}
			r_end_frame(viewport_render_target);
		}
		
		// de allocate all the frame related data
		arena_pop_to(frame_arena, 0);
	}

	r_close_target(viewport_render_target);
	r_close_context(render_context);

	wm_close_window(viewport);
	wm_close_connection();

	arena_release(app_arena);
	arena_release(frame_arena);

	return 0;
}


S32 dual_window(U64 argc, U8** argv){
	(void) argc;
	(void) argv;
	
	Arena* app_arena = arena_alloc();

	wm_create_connection(WM_ConfigFlag_None);
	
	// Viewport 
	WM_Window viewport = wm_create_window(app_arena, 1000, 1000, Str8Lit("Viewport"));
	R_Target* viewport_render_target = r_create_target(app_arena, wm_get_native_handles(app_arena, viewport));

	// Editor
	WM_Window editor = wm_create_window(app_arena, 500, 500, Str8Lit("Editor"));
	R_Target* editor_render_target = r_create_target(app_arena, wm_get_native_handles(app_arena, editor));

	// Shared Rendering Context
	R_Context render_context = r_create_context(app_arena, R_ConfigFlag_None);

	B32 editor_open = BASE_TRUE;
	B32 viewport_open = BASE_TRUE;
	Arena* frame_arena = arena_alloc();

	while(viewport_open || editor_open){

		// viewport event loop
		if(viewport_open){

			// reads the OS events into a doubly linked list
			WM_Event* viewport_events = wm_get_event_list(viewport, frame_arena);

			// traverse the doubly linked list
			for EachNode(e, WM_Event, viewport_events){
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
					viewport_open = BASE_FALSE;
				}break;
				default:{}break;
				}	
			}

		}

		// editor event loop
		if(editor_open){

			// reads the OS events into a doubly linked list
			WM_Event* editor_events = wm_get_event_list(editor, frame_arena);

			// traverse the doubly linked list
			for EachNode(e, WM_Event, editor_events){
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
					editor_open = BASE_FALSE;
				}break;
				default:{}break;
				}	
			}

		}


		if(viewport_open){
			r_begin_frame(viewport_render_target);
			{
				r_start_pass(render_context, viewport_render_target);

				//rendering api must happen here
				glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);


				r_end_pass(render_context);
			}
			r_end_frame(viewport_render_target);
		}
		
		if(editor_open){
			r_begin_frame(editor_render_target);
			{
				r_start_pass(render_context, editor_render_target);

				//rendering api must happen here
				glClearColor(0.3f, 0.3f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);


				r_end_pass(render_context);
			}
			r_end_frame(editor_render_target);

		}

		// de allocate all the frame related data
		arena_pop_to(frame_arena, 0);
	}

	r_close_target(viewport_render_target);
	r_close_target(editor_render_target);
	r_close_context(render_context);

	wm_close_window(viewport);
	wm_close_window(editor);
	wm_close_connection();

	arena_release(app_arena);
	arena_release(frame_arena);

	return 0;
}

int main(int argc, char** argv){os_entry_point(argc, (U8**) argv, &single_window);}
