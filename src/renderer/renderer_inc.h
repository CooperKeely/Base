#ifndef RENDERER_INC_H 
#define RENDERER_INC_H 


# if defined(RENDERER_SOFTWARE_ENABLE)
#  include "software/software_renderer.h"
# elif  defined(RENDERER_VULKAN_ENABLE)
#  include "vulkan/vulkan_renderer.h"
# else
#  error "No renderer selected"
# endif


#endif // RENDERER_INC_H 


