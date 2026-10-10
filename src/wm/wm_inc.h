
#include "wm_core.h"

#if defined(OS_LINUX)
// used to resolve if compositor is wayland or x11 based
B32 wm_should_use_wayland(void);

#  include "wayland/wm_wayland.h"
#  include "x11/wm_x11.h"
#elif defined(OS_WINDOWS)
# error "Unsupported"
#elif defined(OS_MACOS)
# error "Unsupported"
#else
# error "OS not defined"
#endif

