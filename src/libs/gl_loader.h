#ifndef GL_LOADER_H
#define GL_LOADER_H

#include <stddef.h> 

#ifdef __cplusplus
extern "C" {
#endif

// --- Base OpenGL Types ---

#if defined(_WIN32) && !defined(APIENTRY)
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif

typedef unsigned int   GLenum;
typedef unsigned int   GLuint;
typedef int            GLint;
typedef int            GLsizei;
typedef unsigned char  GLboolean;
typedef float          GLfloat;
typedef char           GLchar;
typedef ptrdiff_t      GLsizeiptr;
typedef ptrdiff_t      GLintptr;
typedef unsigned int   GLbitfield;
typedef double         GLdouble;
typedef double         GLclampd;
typedef float          GLclampf;

#define GL_FALSE 0
#define GL_TRUE  1


// --- OpenGL Enums ---
#define GL_ARRAY_BUFFER                0x8892
#define GL_BACK                        0x0405
#define GL_BLEND                       0x0BE2
#define GL_CCW                         0x0901
#define GL_CLAMP_TO_EDGE               0x812F
#define GL_COLOR_ATTACHMENT0           0x8CE0
#define GL_COLOR_BUFFER_BIT            0x00004000
#define GL_COMPILE_STATUS              0x8B81
#define GL_CULL_FACE                   0x0B44
#define GL_CW                          0x0900
#define GL_DEPTH_ATTACHMENT            0x8D00
#define GL_DEPTH_BUFFER_BIT            0x00000100
#define GL_DEPTH_COMPONENT             0x1902
#define GL_DEPTH_TEST                  0x0B71
#define GL_DYNAMIC_DRAW                0x88E8
#define GL_ELEMENT_ARRAY_BUFFER        0x8893
#define GL_FLOAT                       0x1406
#define GL_FRAGMENT_SHADER             0x8B30
#define GL_FRAMEBUFFER                 0x8D40
#define GL_FRONT                       0x0404
#define GL_INFO_LOG_LENGTH             0x8B84
#define GL_INT                         0x1404
#define GL_LEQUAL                      0x0203
#define GL_LESS                        0x0201
#define GL_LINEAR                      0x2601
#define GL_LINES                       0x0001
#define GL_LINK_STATUS                 0x8B82
#define GL_NEAREST                     0x2600
#define GL_ONE_MINUS_SRC_ALPHA         0x0303
#define GL_POINTS                      0x0000
#define GL_REPEAT                      0x2901
#define GL_RGB                         0x1907
#define GL_RGBA                        0x1908
#define GL_RGBA8                       0x8058
#define GL_SRC_ALPHA                   0x0302
#define GL_STATIC_DRAW                 0x88E4
#define GL_STENCIL_BUFFER_BIT          0x00000400
#define GL_TEXTURE_2D                  0x0DE1
#define GL_TEXTURE_3D                  0x806F
#define GL_TEXTURE_CUBE_MAP            0x8513
#define GL_TEXTURE_MAG_FILTER          0x2800
#define GL_TEXTURE_MIN_FILTER          0x2801
#define GL_TEXTURE_WRAP_S              0x2802
#define GL_TEXTURE_WRAP_T              0x2803
#define GL_TRIANGLES                   0x0004
#define GL_UNSIGNED_BYTE               0x1401
#define GL_UNSIGNED_INT                0x1405
#define GL_UNSIGNED_SHORT              0x1403
#define GL_VERTEX_SHADER               0x8B31

// --- Function Pointer Typedefs ---
typedef void (APIENTRY *PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (APIENTRY *PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (APIENTRY *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (APIENTRY *PFNGLBINDBUFFERBASEPROC)(GLenum target, GLuint index, GLuint buffer);
typedef void (APIENTRY *PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
typedef void (APIENTRY *PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (APIENTRY *PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (APIENTRY *PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (APIENTRY *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (APIENTRY *PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
typedef GLenum (APIENTRY *PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef void (APIENTRY *PFNGLCLEARPROC)(GLbitfield mask);
typedef void (APIENTRY *PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRY *PFNGLCLEARDEPTHPROC)(GLdouble depth);
typedef void (APIENTRY *PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef GLuint (APIENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef GLuint (APIENTRY *PFNGLCREATESHADERPROC)(GLenum type);
typedef void (APIENTRY *PFNGLCULLFACEPROC)(GLenum mode);
typedef void (APIENTRY *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
typedef void (APIENTRY *PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint *framebuffers);
typedef void (APIENTRY *PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (APIENTRY *PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (APIENTRY *PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
typedef void (APIENTRY *PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);
typedef void (APIENTRY *PFNGLDEPTHFUNCPROC)(GLenum func);
typedef void (APIENTRY *PFNGLDEPTHMASKPROC)(GLboolean flag);
typedef void (APIENTRY *PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (APIENTRY *PFNGLDISABLEPROC)(GLenum cap);
typedef void (APIENTRY *PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (APIENTRY *PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY *PFNGLDRAWARRAYSINSTANCEDPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
typedef void (APIENTRY *PFNGLDRAWBUFFERSPROC)(GLsizei n, const GLenum *bufs);
typedef void (APIENTRY *PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (APIENTRY *PFNGLDRAWELEMENTSINSTANCEDPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
typedef void (APIENTRY *PFNGLENABLEPROC)(GLenum cap);
typedef void (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (APIENTRY *PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRY *PFNGLFRONTFACEPROC)(GLenum mode);
typedef void (APIENTRY *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void (APIENTRY *PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint *framebuffers);
typedef void (APIENTRY *PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
typedef void (APIENTRY *PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
typedef void (APIENTRY *PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef void (APIENTRY *PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef GLint (APIENTRY *PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
typedef void (APIENTRY *PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (APIENTRY *PFNGLPOLYGONMODEPROC)(GLenum face, GLenum mode);
typedef void (APIENTRY *PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (APIENTRY *PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRY *PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (APIENTRY *PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (APIENTRY *PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (APIENTRY *PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRY *PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY *PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRY *PFNGLUNIFORMMATRIX3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY *PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY *PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (APIENTRY *PFNGLVERTEXATTRIBIPOINTERPROC)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (APIENTRY *PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);

// --- Loader Callback Type ---
typedef void* (*GL_LoaderProc)(const char* name);

// --- X-Macro List ---
#define GL_FUNCTIONS\
	X(PFNGLACTIVETEXTUREPROC        , glActiveTexture) \
	X(PFNGLATTACHSHADERPROC         , glAttachShader) \
	X(PFNGLBINDBUFFERPROC           , glBindBuffer) \
	X(PFNGLBINDBUFFERBASEPROC       , glBindBufferBase) \
	X(PFNGLBINDFRAMEBUFFERPROC      , glBindFramebuffer) \
	X(PFNGLBINDTEXTUREPROC          , glBindTexture) \
	X(PFNGLBINDVERTEXARRAYPROC      , glBindVertexArray) \
	X(PFNGLBLENDFUNCPROC            , glBlendFunc) \
	X(PFNGLBUFFERDATAPROC           , glBufferData) \
	X(PFNGLBUFFERSUBDATAPROC        , glBufferSubData) \
	X(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus) \
	X(PFNGLCLEARPROC                , glClear) \
	X(PFNGLCLEARCOLORPROC           , glClearColor) \
	X(PFNGLCLEARDEPTHPROC           , glClearDepth) \
	X(PFNGLCOMPILESHADERPROC        , glCompileShader) \
	X(PFNGLCREATEPROGRAMPROC        , glCreateProgram) \
	X(PFNGLCREATESHADERPROC         , glCreateShader) \
	X(PFNGLCULLFACEPROC             , glCullFace) \
	X(PFNGLDELETEBUFFERSPROC        , glDeleteBuffers) \
	X(PFNGLDELETEFRAMEBUFFERSPROC   , glDeleteFramebuffers) \
	X(PFNGLDELETEPROGRAMPROC        , glDeleteProgram) \
	X(PFNGLDELETESHADERPROC         , glDeleteShader) \
	X(PFNGLDELETETEXTURESPROC       , glDeleteTextures) \
	X(PFNGLDELETEVERTEXARRAYSPROC   , glDeleteVertexArrays) \
	X(PFNGLDEPTHFUNCPROC            , glDepthFunc) \
	X(PFNGLDEPTHMASKPROC            , glDepthMask) \
	X(PFNGLDETACHSHADERPROC         , glDetachShader) \
	X(PFNGLDISABLEPROC              , glDisable) \
	X(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray) \
	X(PFNGLDRAWARRAYSPROC           , glDrawArrays) \
	X(PFNGLDRAWARRAYSINSTANCEDPROC  , glDrawArraysInstanced) \
	X(PFNGLDRAWBUFFERSPROC          , glDrawBuffers) \
	X(PFNGLDRAWELEMENTSPROC         , glDrawElements) \
	X(PFNGLDRAWELEMENTSINSTANCEDPROC, glDrawElementsInstanced) \
	X(PFNGLENABLEPROC               , glEnable) \
	X(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray) \
	X(PFNGLFRAMEBUFFERTEXTURE2DPROC , glFramebufferTexture2D) \
	X(PFNGLFRONTFACEPROC            , glFrontFace) \
	X(PFNGLGENBUFFERSPROC           , glGenBuffers) \
	X(PFNGLGENFRAMEBUFFERSPROC      , glGenFramebuffers) \
	X(PFNGLGENTEXTURESPROC          , glGenTextures) \
	X(PFNGLGENVERTEXARRAYSPROC      , glGenVertexArrays) \
	X(PFNGLGENERATEMIPMAPPROC       , glGenerateMipmap) \
	X(PFNGLGETPROGRAMINFOLOGPROC    , glGetProgramInfoLog) \
	X(PFNGLGETPROGRAMIVPROC         , glGetProgramiv) \
	X(PFNGLGETSHADERINFOLOGPROC     , glGetShaderInfoLog) \
	X(PFNGLGETSHADERIVPROC          , glGetShaderiv) \
	X(PFNGLGETUNIFORMLOCATIONPROC   , glGetUniformLocation) \
	X(PFNGLLINKPROGRAMPROC          , glLinkProgram) \
	X(PFNGLPOLYGONMODEPROC          , glPolygonMode) \
	X(PFNGLSHADERSOURCEPROC         , glShaderSource) \
	X(PFNGLTEXIMAGE2DPROC           , glTexImage2D) \
	X(PFNGLTEXPARAMETERFPROC        , glTexParameterf) \
	X(PFNGLTEXPARAMETERIPROC        , glTexParameteri) \
	X(PFNGLTEXSUBIMAGE2DPROC        , glTexSubImage2D) \
	X(PFNGLUNIFORM1FPROC            , glUniform1f) \
	X(PFNGLUNIFORM1IPROC            , glUniform1i) \
	X(PFNGLUNIFORM2FPROC            , glUniform2f) \
	X(PFNGLUNIFORM3FPROC            , glUniform3f) \
	X(PFNGLUNIFORM4FPROC            , glUniform4f) \
	X(PFNGLUNIFORMMATRIX3FVPROC     , glUniformMatrix3fv) \
	X(PFNGLUNIFORMMATRIX4FVPROC     , glUniformMatrix4fv) \
	X(PFNGLUSEPROGRAMPROC           , glUseProgram) \
	X(PFNGLVERTEXATTRIBIPOINTERPROC , glVertexAttribIPointer) \
	X(PFNGLVERTEXATTRIBPOINTERPROC  , glVertexAttribPointer) \
	X(PFNGLVIEWPORTPROC             , glViewport)

#define X(type, name) extern type name;
GL_FUNCTIONS
#undef X

int gl_loader_init(GL_LoaderProc load_proc);

#ifdef __cplusplus
}
#endif

#endif // GL_LOADER_H 

// =========================================================================
#ifdef GL_LOADER_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#endif

#define X(type, name) type name = NULL;
GL_FUNCTIONS
#undef X

int gl_loader_init(GL_LoaderProc load_proc) {
    int success = 1;
    if (!load_proc) return 0;

    #define X(type, name) \
        do { \
            name = (type)load_proc(#name); \
            if (!name) { success = 0; } \
        } while(0);

    GL_FUNCTIONS
    #undef X

    return success; 
}

#ifdef __cplusplus
}
#endif
#endif // GL_LOADER_IMPLEMENTATION
