
#include "wm_core.c"

#if defined(OS_LINUX)

global B32 wm_use_wayland = BASE_FALSE;

B32 wm_should_use_wayland(void){
	Str8 env_result = os_get_env_variable(Str8("WAYLAND_DISPLAY");
	if(env_result.size != 0){ return BASE_TRUE; }

	// fallback to second check
	Str8 session_type = os_get_env_variable(Str8("XDG_SESSION_TYPE");
	if(str8_cmp(session_type, Str8("wayland"))) { return BASE_TRUE; }

	return BASE_FALSE;
}

# include "wayland/wm_wayland.c"
# include "x11/wm_x11.c"

#elif defined(OS_WINDOWS)
# error "Unsupported"
#elif defined(OS_MACOS)
# error "Unsupported"
#else
# error "OS not defined"
#endif

