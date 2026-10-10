#ifndef BASE_INC_H
#define BASE_INC_H

#include "base_context_cracking.h"

#include "base_core.h"
#include "base_math.h"
#include "base_arena.h"
#include "base_string.h"
#include "base_log.h"
#include "base_thread_ctx.h"

#include "os/os_core.h"
#if defined(OS_LINUX)
# include "os/linux/os_linux.h"
#elif defined(OS_WINDOWS)
# error "unsupported OS" 
#elif defined(OS_MACOS)
# error "unsupported OS" 
#else
# error "No OS core specified" 
#endif


#endif // BASE_INC_H
