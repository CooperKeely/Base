
#include "core/os_core.h"

#ifdef OS_LINUX
# include "core/os_linux/os_linux.h"
#elifdef OS_WINDOWS
# include "core/os_windows/os_windows.h"
#elifdef OS_MACOS
# include "core/os_macos/os_macos.h"
#endif

#ifdef OS_GFX_ENABLE 
# include "gfx/os_gfx.h"
# ifdef OS_LINUX_WAYLAND
#  include "gfx/os_linux/wayland/os_linux_wayland_gfx.h"
# elifdef OS_LINUX_X11
#  include "gfx/os_linux/x11/os_linux_x11_gfx.h"
# elifdef OS_WINDOWS
#  include "core/os_windows/os_windows_gfx.h"
# elifdef OS_MACOS
#  include "core/os_macos/os_macos_gfx.h"
# endif
#endif


