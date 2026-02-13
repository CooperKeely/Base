
#include "core/os_core.c"

#if OS_LINUX
# include "core/os_linux/os_linux.c"
#elif OS_WINDOWS
# include "core/os_windows/os_windows.c"
#elif OS_MACOS
# include "core/os_macos/os_macos.c"
#endif

#ifdef OS_GFX_ENABLE 
# include "gfx/os_gfx.c"
# if OS_LINUX
#  include "gfx/os_linux/os_linux_gfx.c"
# elif OS_WINDOWS
#  include "core/os_windows/os_windows_gfx.c"
# elif OS_MACOS
#  include "core/os_macos/os_macos_gfx.c"
# endif
#endif


