#include "core/os_core.h"

#if defined(OS_LINUX)
# include "core/os_linux/os_linux.h"
#else
# error "No OS core specified" 
#endif

#if defined(OS_GFX_ENABLE)
# include "gfx/os_gfx.h"
# if defined(OS_LINUX)
#  undef global
#  include "gfx/wayland/os_linux_wayland_gfx.h"
#  define global static
# else
#  error "No OS graphics specified"
# endif
#endif


