#include "target/r_target_core.h"
#include "target/r_target_core.c"

#if defined(OS_LINUX)

# include "target/x11/r_target_x11.h"
# include "target/x11/r_target_x11.c"

# include "target/wayland/r_target_wayland.h"
# include "target/wayland/r_target_wayland.c"

#elif defined(OS_WINDOWS)

# include "target/win32/r_target_win32.h"
# include "target/win32/r_target_win32.c"

#elif defined(OS_MACOS)

# include "target/cocoa/r_target_cocoa.h"
# include "target/cocoa/r_target_cocoa.c"

#endif



#include "gfx/r_core.h"
#include "gfx/r_core.c"

#if defined(OS_LINUX)

# if defined(ENABLE_RENDER_BACKEND_OPENGL)
#  include "gfx/opengl/r_opengl.h" 
#  include "gfx/opengl/r_opengl.c" 
# elif defined(ENABLE_RENDER_BACKEND_VULKAN)
#  include "gfx/vulkan/r_vulkan.h"
#  include "gfx/vulkan/r_vulkan.c"
# else
#  error "Unsupported Linux Graphics Backend"
# endif

#elif defined(OS_WINDOWS)

# if defined(ENABLE_RENDER_BACKEND_OPENGL)
#  include "gfx/opengl/r_opengl.h" 
#  include "gfx/opengl/r_opengl.c" 
# elif defined(ENABLE_RENDER_BACKEND_VULKAN)
#  include "gfx/vulkan/r_vulkan.h"
#  include "gfx/vulkan/r_vulkan.c"
# elif defined(ENABLE_RENDER_BACKEND_DX12)
#  include "gfx/dx12/r_dx12.h"
#  include "gfx/dx12/r_dx12.c"
# else
#  error "Unsupported Windows Graphics Backend"
# endif


#elif defined(OS_MACOS)

# if defined(ENABLE_RENDER_BACKEND_OPENGL)
#  include "gfx/opengl/r_opengl.h" 
#  include "gfx/opengl/r_opengl.c" 
# elif defined(ENABLE_RENDER_BACKEND_VULKAN)
#  include "gfx/vulkan/r_vulkan.h"
#  include "gfx/vulkan/r_vulkan.c"
# elif defined(ENABLE_RENDER_BACKEND_METAL)
#  include "gfx/metal/r_metal.h"
#  include "gfx/metal/r_metal.c"
# else
#  error "Unsupported Windows Graphics Backend"
# endif

#endif

