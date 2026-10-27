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


///////////////////////////////////////
/// cjk: window manager test main

#include "base/base_inc.h"
#include "wm/wm_inc.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>

// opengl function loader
#define GL_LOADER_IMPLEMENTATION
#include "./libs/gl_loader.h"

typedef struct {
    EGLint attribute;
    const char *name;
} EGLAttributeName;

static const EGLAttributeName egl_attrib_list[] = {
    { EGL_BUFFER_SIZE,             "EGL_BUFFER_SIZE" },
    { EGL_ALPHA_SIZE,              "EGL_ALPHA_SIZE" },
    { EGL_BLUE_SIZE,               "EGL_BLUE_SIZE" },
    { EGL_GREEN_SIZE,              "EGL_GREEN_SIZE" },
    { EGL_RED_SIZE,                "EGL_RED_SIZE" },
    { EGL_DEPTH_SIZE,              "EGL_DEPTH_SIZE" },
    { EGL_STENCIL_SIZE,            "EGL_STENCIL_SIZE" },
    { EGL_CONFIG_CAVEAT,           "EGL_CONFIG_CAVEAT" },
    { EGL_CONFIG_ID,               "EGL_CONFIG_ID" },
    { EGL_LEVEL,                   "EGL_LEVEL" },
    { EGL_MAX_PBUFFER_HEIGHT,      "EGL_MAX_PBUFFER_HEIGHT" },
    { EGL_MAX_PBUFFER_PIXELS,      "EGL_MAX_PBUFFER_PIXELS" },
    { EGL_MAX_PBUFFER_WIDTH,       "EGL_MAX_PBUFFER_WIDTH" },
    { EGL_NATIVE_RENDERABLE,       "EGL_NATIVE_RENDERABLE" },
    { EGL_NATIVE_VISUAL_ID,        "EGL_NATIVE_VISUAL_ID" },
    { EGL_NATIVE_VISUAL_TYPE,      "EGL_NATIVE_VISUAL_TYPE" },
    { EGL_SAMPLES,                 "EGL_SAMPLES" },
    { EGL_SAMPLE_BUFFERS,          "EGL_SAMPLE_BUFFERS" },
    { EGL_SURFACE_TYPE,            "EGL_SURFACE_TYPE" },
    { EGL_TRANSPARENT_TYPE,        "EGL_TRANSPARENT_TYPE" },
    { EGL_TRANSPARENT_BLUE_VALUE,  "EGL_TRANSPARENT_BLUE_VALUE" },
    { EGL_TRANSPARENT_GREEN_VALUE, "EGL_TRANSPARENT_GREEN_VALUE" },
    { EGL_TRANSPARENT_RED_VALUE,   "EGL_TRANSPARENT_RED_VALUE" },
    { EGL_BIND_TO_TEXTURE_RGB,     "EGL_BIND_TO_TEXTURE_RGB" },
    { EGL_BIND_TO_TEXTURE_RGBA,    "EGL_BIND_TO_TEXTURE_RGBA" },
    { EGL_MIN_SWAP_INTERVAL,       "EGL_MIN_SWAP_INTERVAL" },
    { EGL_MAX_SWAP_INTERVAL,       "EGL_MAX_SWAP_INTERVAL" },
    { EGL_LUMINANCE_SIZE,          "EGL_LUMINANCE_SIZE" },
    { EGL_ALPHA_MASK_SIZE,         "EGL_ALPHA_MASK_SIZE" },
    { EGL_COLOR_BUFFER_TYPE,       "EGL_COLOR_BUFFER_TYPE" },
    { EGL_RENDERABLE_TYPE,         "EGL_RENDERABLE_TYPE" },
    { EGL_CONFORMANT,              "EGL_CONFORMANT" }
};

void egl_print_config(EGLDisplay display, EGLConfig config){

	EGLBoolean err;

	EGLint config_id;
	err = eglGetConfigAttrib(display, config, EGL_CONFIG_ID, &config_id);
	if(!err) { LogWarning("Couldn't retrieve EGL_CONFIG_ID"); }


	LogDebug("\n================ EGL Config ID: %d ================", config_id);

	U32 number_of_attributes = ArrayCount(egl_attrib_list);
	for EachIndex(j, number_of_attributes) {
		EGLint value = 0;
		err = eglGetConfigAttrib(display, config, egl_attrib_list[j].attribute, &value);
		if (err) { LogDebug("  %-30s : %d (0x%X)", egl_attrib_list[j].name, value, value); } 
		else { LogWarning("  %-30s : <Unavailable / Error 0x%X>", egl_attrib_list[j].name, eglGetError()); }
	}
}


S32 wm_test_main(U64 argc, U8** argv){
	(void) argc;
	(void) argv;
	
	Arena* app_arena = arena_alloc();
	Arena* frame_arena = arena_alloc();

	WM_Display wm_display = wm_init(app_arena, WM_ConfigFlag_Resizeable);
	WM_Window wm_window = wm_window_create(app_arena, 1000, 1000, Str8Lit("Software Renderer"));
	
	// unpack for use in egl
	WM_WaylandDisplay* way_display = Handle_Unpack(WM_WaylandDisplay*, wm_display);
	WM_WaylandWindow* way_window = Handle_Unpack(WM_WaylandWindow*, wm_window);


	EGLint major;
	EGLint minor;
	EGLConfig* configs;
	EGLint configs_size = 3;
	EGLint num_configs;

	EGLint ctx_attributes[] = { EGL_NONE };
	EGLint attributes[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_NONE
	};

	EGLBoolean err;

	EGLDisplay egl_display = eglGetDisplay(way_display->wl_display);
	if(egl_display == EGL_NO_DISPLAY){ 
		LogError("Failed to get EGL display"); 
	}else{  LogDebug("Connected to EGL display"); }

	err = eglInitialize(egl_display, &major, &minor); 
	if(!err){ LogError("Failed to initialize EGL: (%d)", eglGetError()); }
	else {   LogDebug("Successfully initialized EGL display"); }

	/* 
	 NOTE: This code queries all avaible configurations and prints them

	err = eglGetConfigs(egl_display, NULL, 0, &num_configs);
	if(!err){ LogError("Failed to get number of configs: (%d)", eglGetError()); }
	else {   LogDebug("Successfully got number of EGL configs"); }

	configs = ArenaPushArray(app_arena, EGLConfig, num_configs);

	EGLint retrieved_configs = 0;
	err = eglGetConfigs(egl_display, configs, num_configs, &retrieved_configs);
	if(!err){ LogError("Failed to get configs: (%d)", eglGetError()); }
	else {   LogDebug("Successfully got EGL configs"); }
	Assert(retrieved_configs == num_configs);

	for EachIndex(config, retrieved_configs){
		egl_print_config(egl_display, configs[config]);	
	} 

	*/
	
	// retrieves 3 of the best matched configs
	configs = ArenaPushArray(app_arena, EGLConfig, configs_size);	
	err = eglChooseConfig(egl_display, attributes, configs, configs_size, &num_configs);
	if(!err) { LogError("Failed to choose config: %d", eglGetError()); }
	else { LogDebug("Successfully matched %d configs", num_configs); }

	Assert(configs[0] != EGL_NONE);
	EGLConfig config = configs[0];

	err = eglBindAPI(EGL_OPENGL_API);
	if(!err) { LogError("Failed to bind to egl OpenGL api"); }
	else { LogDebug("Successfully attached to egl OpenGl Api"); }

	EGLContext egl_context = eglCreateContext(egl_display, config, EGL_NO_CONTEXT, ctx_attributes);
	if (egl_context == EGL_NO_CONTEXT){ LogError("Failed to attach to egl context : %d", eglGetError()); }	
	else { LogDebug("Successfully attached to egl context"); }


	// create egl surface
	EGLSurface egl_surface = eglCreateWindowSurface(egl_display, config, way_window->wl_egl_window, NULL);
	if (egl_surface == EGL_NO_SURFACE){ LogError("Failed to attach to native window : %d", eglGetError()); }	
	else { LogDebug("Successfully attached to native window"); }

	// make the current context
	err = eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);
	if (!err) { LogError("Failed to make the current context: %d", eglGetError()); }
	else { LogDebug("Successfully made the current context"); }

	gl_loader_init((GL_LoaderProc) eglGetProcAddress);

	

	B32 running = BASE_TRUE;
	while(running){
		// reads the OS events into a doubly linked list using
		WM_Event* event_head = wm_event_get_list(wm_window, frame_arena);

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

		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		eglSwapBuffers(egl_display, egl_surface);


		// de allocate all the frame related data
		arena_pop_to(frame_arena, 0);
	}

	err = eglDestroyContext(egl_display, egl_context);
	if(!err){ LogError("Failed to destroy EGL context: (%d)", eglGetError()); }
	else { LogDebug("Successfully destroyed EGL context"); }

	err = eglTerminate(egl_display); 
	if(!err){ LogError("Failed to terminate EGL: (%d)", eglGetError()); }
	else { LogDebug("Successfully terminated EGL display"); }

	wm_close(wm_display);

	arena_release(app_arena);
	arena_release(frame_arena);

	return 0;

}

///////////////////////////////////////
/// cjk: api example


#include "base/base_inc.h"
#include "wm/wm_inc.h"
#include "gfx/gfx_inc.h"
#include "render/r_inc.h"


S32 example_api(U64 argc, U8** argv){
	(void) argc;
	(void) argv;
	
	Arena* app_arena = arena_alloc();


	WM_Display display = wm_init(app_arena, WM_ConfigFlag_Resizeable);
	
	// Viewport 
	WM_Window window1 = wm_window_create(app_arena, 1000, 1000, Str8Lit("Viewport"));
	GFX_Surface surface2 = gfx_surface_create(window1, GFX_BackendType_OpenGL, NULL);
	B32 viewport_open = BASE_TRUE;

	// Editor window initialization
	WM_Window window2 = wm_window_create(app_arena, 500, 500, Str8Lit("Editor"));
	GFX_Surface surface2 = gfx_surface_create(window2, GFX_BackendType_OpenGL, NULL);
	B32 editor_open = BASE_TRUE;
	
	// Shared Rendering Context
	R_Context render_context = r_init(app_arena, R_ConfigFlag_None);

	Arena* frame_arena = arena_alloc();

	while(viewport_open || editor_open){
		wm_pump_events(display);

		// viewport event loop
		if(viewport_open){

			// reads the OS events into a doubly linked list
			WM_Event* event_head = wm_event_get_list(window1, frame_arena);

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
					viewport_open = BASE_FALSE;
				}break;
				default:{}break;
				}	
			}

		}

		// editor event loop
		if(editor_open){

			// reads the OS events into a doubly linked list
			WM_Event* event_head = wm_event_get_list(window2, frame_arena);

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
					editor_open = BASE_FALSE;
				}break;
				default:{}break;
				}	
			}

		}


		if(viewport_open){
			gfx_make_current(surface1);
			gfx_begin_frame(surface1);
			{
				r_start_pass(r_ctx, surface);

				//rendering api must happen here

				r_end_pass(r_ctx);
			}
			gfx_submit_frame(surface1);

		}
		
		if(editor_open){
			gfx_make_current(surface2);
			gfx_begin_frame(surface);
			{	
				r_start_pass(r_ctx, surface);

				//rendering api must happen here

				r_end_pass(r_ctx);
			}
			gfx_submit_frame(surface2);

		}

		// de allocate all the frame related data
		arena_pop_to(frame_arena, 0);
	}

	r_close(render_context);
	gfx_close(surface);
	wm_close(display);

	arena_release(app_arena);
	arena_release(frame_arena);

	return 0;
}

S32 entry_point(U64 argc, U8** argv){
	wm_test_main(argc, argv);	

}

int main(int argc, char** argv){os_entry_point(argc, (U8**) argv, &entry_point);}
