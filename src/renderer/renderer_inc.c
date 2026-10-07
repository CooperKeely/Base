#ifndef RENDERER_INC_C
#define RENDERER_INC_C 

# include "renderer.c"
# if  defined(RENDERER_VULKAN_ENABLE)
#  include "vulkan/vulkan_renderer.c"
# else
#  error "No renderer selected"
# endif


#endif // RENDERER_INC_C 


