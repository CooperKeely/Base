// Header Guard
#ifndef GL_LOADER_H
#define GL_LOADER_H

#include <EGL/egl.h>
#include <GL/gl.h>
#include <GL/glext.h>

#define GL_FUNCTIONS\
	X(PFNGLCREATEPROGRAMPROC,     glCreateProgram)     \
	X(PFNGLCREATESHADERPROC,      glCreateShader)      \
	X(PFNGLSHADERSOURCEPROC,      glShaderSource)      \
	X(PFNGLCOMPILESHADERPROC,     glCompileShader)     \
	X(PFNGLATTACHSHADERPROC,      glAttachShader)      \
	X(PFNGLLINKPROGRAMPROC,       glLinkProgram)       \
	X(PFNGLUSEPROGRAMPROC,        glUseProgram)        \
	X(PFNGLGENVERTEXARRAYSPROC,   glGenVertexArrays)   \
	X(PFNGLBINDVERTEXARRAYPROC,   glBindVertexArray)   \
	X(PFNGLGENBUFFERSPROC,        glGenBuffers)        \
	X(PFNGLBINDBUFFERPROC,        glBindBuffer)        \
	X(PFNGLBUFFERDATAPROC,        glBufferData)        \
	X(PFNGLGETUNIFORMLOCATIONPROC,glGetUniformLocation)\
	X(PFNGLUNIFORMMATRIX4FVPROC,  glUniformMatrix4fv)

#define X(type, name) extern type name;
GL_FUNCTIONS
#undef X

int gl_loader_load_opengl_extensions(void);

#endif // GL_LOADER_H 

#ifdef GL_LOADER_IMPLEMENTATION

#define X(type, name) type name = NULL;
GL_FUNCTIONS
#undef X

int gl_loader_load_opengl_extensions(void){
// (cjk): load the functions and cause a runtime seg fault if function load failed	
#define X(type, name) \
	do{\
		name = (type)eglGetProcAddress(#name);\
		if(!name){*(volatile int*) 0 = 0;}\
	} while(0);

	GL_FUNCTIONS

#undef X
	return 1;
}

#endif //GL_LOADER_IMPLEMENTATION

