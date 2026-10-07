#include "core/os_core.h"

#if defined(OS_LINUX)
# include "core/os_linux/os_linux.h"
#elif defined(OS_WINDOWS)
# include "core/os_windows/os_windows.h"
#elif defined(OS_MACOS)
# include "core/os_macos/os_macos.h"
#else
# error "No OS core specified" 
#endif

#if defined(OS_GFX_ENABLE)
# include "gfx/os_gfx.h"
# if defined(OS_LINUX_WAYLAND) && defined(OS_LINUX)
#  undef global
#  include "gfx/os_linux/wayland/os_linux_wayland_gfx.h"
#  define global static
# elif defined(OS_LINUX_X11) && defined(OS_LINUX)
#  include "gfx/os_linux/x11/os_linux_x11_gfx.h"
# elif defined(OS_WINDOWS)
#  include "core/os_windows/os_windows_gfx.h"
# elif defined(OS_MACOS)
#  include "core/os_macos/os_macos_gfx.h"
# else
#  error "No OS graphics specified"
# endif
#endif


