#ifndef RENDERER_INC_C
#define RENDERER_INC_C 


# if defined(RENDERER_SOFTWARE_ENABLE)
#  include "software/software_renderer.c"
# elif  defined(RENDERER_VULKAN_ENABLE)
#  include "vulkan/vulkan_renderer.c"
# else
#  error "No renderer selected"
# endif


#endif // RENDERER_INC_C 


