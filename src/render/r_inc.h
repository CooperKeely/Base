#include "r_core.h"
#if defined(RENDER_ENABLE_OPENGL)
# include "opengl/r_opengl.h" 
#elif defined(RENDER_ENABLE_VULKAN)
# error "Vulkan not supported yet"
#endif

#include "r_core.c"
#if defined(RENDER_ENABLE_OPENGL)
# include "opengl/r_opengl.h" 
#elif defined(RENDER_ENABLE_VULKAN)
# error "Vulkan not supported yet"
#endif
