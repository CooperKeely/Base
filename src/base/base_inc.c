#include "base_core.c"
#include "base_math.c"
#include "base_arena.c"
#include "base_string.c"
#include "base_log.c"
#include "base_thread_ctx.c"

#include "os/os_core.c"
#if defined(OS_LINUX)
# include "os/linux/os_linux.c"
#elif defined(OS_WINDOWS)
# error "unsupported OS" 
#elif defined(OS_MACOS)
# error "unsupported OS" 
#else
# error "No OS core specified" 
#endif


