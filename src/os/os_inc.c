
#include "core/os_core.c"

#ifdef OS_LINUX
# include "core/os_linux/os_linux.c"
#elifdef OS_WINDOWS
# include "core/os_windows/os_windows.c"
#elifdef OS_MACOS
# include "core/os_macos/os_macos.c"
#endif

#ifdef OS_GFX_ENABLE 
# include "gfx/os_gfx.c"
# ifdef OS_LINUX_WAYLAND
#  include "gfx/os_linux/wayland/os_linux_wayland_gfx.c"
# elifdef OS_LINUX_X11
#  include "gfx/os_linux/x11/os_linux_x11_gfx.c"
# elifdef OS_WINDOWS
#  include "core/os_windows/os_windows_gfx.c"
# elifdef OS_MACOS
#  include "core/os_macos/os_macos_gfx.c"
# endif
#endif

