///////////////////////////////////////
/// cjk: header files include

#define OS_GFX_ENABLE
#define RENDERER_SOFTWARE_ENABLE
#define FORMAT_OBJ_ENABLE

#include "base/base_inc.h"
#include "os/os_inc.h"
#include "format/format_inc.h"
#include "renderer/renderer_inc.h"

#include "base/base_inc.c"
#include "os/os_inc.c"
#include "format/format_inc.c"
#include "renderer/renderer_inc.c"


S32 entry_point(U64 argc, U8** argv){
	(void) argc;
	(void) argv;
	
	return 0;
}


int main(int argc, char** argv){os_entry_point(argc, (U8**) argv, &entry_point);}
