
#include "wm_core.h"

#if defined(OS_LINUX)
// used to resolve if compositor is wayland or x11 based
B32 wm_should_use_wayland(void);

# undef global
# include "wayland/wm_wayland.h"
# define global static

# include "x11/wm_x11.h"
#elif defined(OS_WINDOWS)
# error "Unsupported"
#elif defined(OS_MACOS)
# error "Unsupported"
#else
# error "OS not defined"
#endif

#include "wm_core.c"

#if defined(OS_LINUX)


B32 wm_should_use_wayland(void){
	Str8 env_result = os_get_env_variable(Str8Lit("WAYLAND_DISPLAY"));
	if(env_result.size != 0){ return BASE_TRUE; }

	// fallback to second check
	Str8 session_type = os_get_env_variable(Str8Lit("XDG_SESSION_TYPE"));
	if(str8_cmp(session_type, Str8Lit("wayland"))) { return BASE_TRUE; }

	return BASE_FALSE;
}

# undef global
# include "wayland/wm_wayland.c"
# define global static

# include "x11/wm_x11.c"

#elif defined(OS_WINDOWS)
# error "Unsupported"
#elif defined(OS_MACOS)
# error "Unsupported"
#else
# error "OS not defined"
#endif

