
#include "core/os_core.h"

#if OS_LINUX
# include "core/os_linux/os_linux.h"
#elif OS_WINDOWS
# include "core/os_windows/os_windows.h"
#elif OS_MACOS
# include "core/os_macos/os_macos.h"
#endif

#ifdef OS_GFX_ENABLE 
# include "gfx/os_gfx.h"
# if OS_LINUX
#  include "gfx/os_linux/os_linux_gfx.h"
# elif OS_WINDOWS
#  include "core/os_windows/os_windows_gfx.h"
# elif OS_MACOS
#  include "core/os_macos/os_macos_gfx.h"
# endif
#endif


