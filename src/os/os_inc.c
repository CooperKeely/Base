#include "core/os_core.c"

#if defined(OS_LINUX)
# include "core/os_linux/os_linux.c"
#else
# error "No OS core specified" 
#endif

#if defined(OS_GFX_ENABLE)
# include "gfx/os_gfx.c"
# if defined(OS_LINUX)
#  undef global
#  include "gfx/wayland/os_linux_wayland_gfx.c"
#  define global static
# else
#  error "No OS graphics specified"
# endif
#endif

