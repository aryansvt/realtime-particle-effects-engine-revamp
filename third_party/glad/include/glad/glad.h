#pragma once

// minimal glad style loader
// only includes the subset of opengl used by this project
// extended with framebuffer object support for hdr post processing
// swap this folder with a full glad export if you want

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include <GL/gl.h>

#ifndef APIENTRY
#define APIENTRY __stdcall
#endif

#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

#ifndef GLchar
typedef char GLchar;
#endif

#ifndef GLintptr
typedef ptrdiff_t GLintptr;
#endif

#ifndef GLsizeiptr
typedef ptrdiff_t GLsizeiptr;
#endif


#ifdef __cplusplus
extern "C" {
#endif

// ---- missing constants on some windows headers ----
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif
#ifndef GL_DYNAMIC_DRAW
#define GL_DYNAMIC_DRAW 0x88E8
#endif
#ifndef GL_STREAM_DRAW
#define GL_STREAM_DRAW 0x88E0
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH 0x8B84
#endif
#ifndef GL_TEXTURE0
#define GL_TEXTURE0 0x84C0
#endif
#ifndef GL_TEXTURE1
#define GL_TEXTURE1 0x84C1
#endif
#ifndef GL_TEXTURE2
#define GL_TEXTURE2 0x84C2
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#ifndef GL_FUNC_ADD
#define GL_FUNC_ADD 0x8006
#endif
#ifndef GL_BLEND_EQUATION
#define GL_BLEND_EQUATION 0x8009
#endif
#ifndef GL_BLEND_EQUATION_RGB
#define GL_BLEND_EQUATION_RGB 0x8009
#endif
#ifndef GL_BLEND_EQUATION_ALPHA
#define GL_BLEND_EQUATION_ALPHA 0x883D
#endif
#ifndef GL_FUNC_REVERSE_SUBTRACT
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#endif
#ifndef GL_DEPTH_TEST
#define GL_DEPTH_TEST 0x0B71
#endif
#ifndef GL_CULL_FACE
#define GL_CULL_FACE 0x0B44
#endif
#ifndef GL_TEXTURE_WRAP_S
#define GL_TEXTURE_WRAP_S 0x2802
#endif
#ifndef GL_TEXTURE_WRAP_T
#define GL_TEXTURE_WRAP_T 0x2803
#endif
#ifndef GL_TEXTURE_MIN_FILTER
#define GL_TEXTURE_MIN_FILTER 0x2801
#endif
#ifndef GL_TEXTURE_MAG_FILTER
#define GL_TEXTURE_MAG_FILTER 0x2800
#endif
#ifndef GL_REPEAT
#define GL_REPEAT 0x2901
#endif
#ifndef GL_RGBA8
#define GL_RGBA8 0x8058
#endif
#ifndef GL_SRGB8_ALPHA8
#define GL_SRGB8_ALPHA8 0x8C43
#endif
#ifndef GL_UNSIGNED_INT
#define GL_UNSIGNED_INT 0x1405
#endif
#ifndef GL_UNSIGNED_SHORT
#define GL_UNSIGNED_SHORT 0x1403
#endif

// ---- fbo and float texture constants ----
#ifndef GL_FRAMEBUFFER
#define GL_FRAMEBUFFER 0x8D40
#endif
#ifndef GL_DRAW_FRAMEBUFFER
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#endif
#ifndef GL_READ_FRAMEBUFFER
#define GL_READ_FRAMEBUFFER 0x8CA8
#endif
#ifndef GL_COLOR_ATTACHMENT0
#define GL_COLOR_ATTACHMENT0 0x8CE0
#endif
#ifndef GL_DEPTH_ATTACHMENT
#define GL_DEPTH_ATTACHMENT 0x8D00
#endif
#ifndef GL_FRAMEBUFFER_COMPLETE
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#endif
#ifndef GL_DEPTH_COMPONENT
#define GL_DEPTH_COMPONENT 0x1902
#endif
#ifndef GL_DEPTH_COMPONENT24
#define GL_DEPTH_COMPONENT24 0x81A6
#endif
#ifndef GL_DEPTH_COMPONENT32F
#define GL_DEPTH_COMPONENT32F 0x8CAC
#endif
#ifndef GL_RGBA16F
#define GL_RGBA16F 0x881A
#endif
#ifndef GL_RGB16F
#define GL_RGB16F 0x881B
#endif
#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif
#ifndef GL_RGB
#define GL_RGB 0x1907
#endif
#ifndef GL_FLOAT
#define GL_FLOAT 0x1406
#endif
#ifndef GL_HALF_FLOAT
#define GL_HALF_FLOAT 0x140B
#endif
#ifndef GL_LINEAR_MIPMAP_LINEAR
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#endif

// ---- glad loader signature ----
typedef void* (*GLADloadproc)(const char* name);

// ---- function pointer declarations ----
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint* arrays);
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint* arrays);

typedef void (APIENTRYP PFNGLGENBUFFERSPROC)(GLsizei n, GLuint* buffers);
typedef void (APIENTRYP PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC)(GLenum target, ptrdiff_t size, const void* data, GLenum usage);
typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC)(GLenum target, ptrdiff_t offset, ptrdiff_t size, const void* data);
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint* buffers);

typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void (APIENTRYP PFNGLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);

typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (APIENTRYP PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);

typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC)(void);
typedef void (APIENTRYP PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (APIENTRYP PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC)(GLuint program);

typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar* name);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (APIENTRYP PFNGLUNIFORM1IPROC)(GLint location, GLint v0);

typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (APIENTRYP PFNGLGENTEXTURESPROC)(GLsizei n, GLuint* textures);
typedef void (APIENTRYP PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (APIENTRYP PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (APIENTRYP PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (APIENTRYP PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef void (APIENTRYP PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint* textures);

typedef void (APIENTRYP PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (APIENTRYP PFNGLBLENDEQUATIONPROC)(GLenum mode);

typedef void (APIENTRYP PFNGLDRAWARRAYSINSTANCEDPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);

// fbo functions
typedef void (APIENTRYP PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint* framebuffers);
typedef void (APIENTRYP PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
typedef void (APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint* framebuffers);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLenum (APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef void (APIENTRYP PFNGLDRAWBUFFERSPROC)(GLsizei n, const GLenum* bufs);

// extern function pointers (glad style)
extern PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays;

extern PFNGLGENBUFFERSPROC glad_glGenBuffers;
extern PFNGLBINDBUFFERPROC glad_glBindBuffer;
extern PFNGLBUFFERDATAPROC glad_glBufferData;
extern PFNGLBUFFERSUBDATAPROC glad_glBufferSubData;
extern PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers;

extern PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer;
extern PFNGLVERTEXATTRIBDIVISORPROC glad_glVertexAttribDivisor;

extern PFNGLCREATESHADERPROC glad_glCreateShader;
extern PFNGLSHADERSOURCEPROC glad_glShaderSource;
extern PFNGLCOMPILESHADERPROC glad_glCompileShader;
extern PFNGLGETSHADERIVPROC glad_glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog;

extern PFNGLCREATEPROGRAMPROC glad_glCreateProgram;
extern PFNGLATTACHSHADERPROC glad_glAttachShader;
extern PFNGLLINKPROGRAMPROC glad_glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glad_glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC glad_glUseProgram;
extern PFNGLDELETESHADERPROC glad_glDeleteShader;
extern PFNGLDELETEPROGRAMPROC glad_glDeleteProgram;

extern PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv;
extern PFNGLUNIFORM3FVPROC glad_glUniform3fv;
extern PFNGLUNIFORM4FVPROC glad_glUniform4fv;
extern PFNGLUNIFORM2FVPROC glad_glUniform2fv;
extern PFNGLUNIFORM1FPROC glad_glUniform1f;
extern PFNGLUNIFORM1IPROC glad_glUniform1i;

extern PFNGLACTIVETEXTUREPROC glad_glActiveTexture;
extern PFNGLGENTEXTURESPROC glad_glGenTextures;
extern PFNGLBINDTEXTUREPROC glad_glBindTexture;
extern PFNGLTEXPARAMETERIPROC glad_glTexParameteri;
extern PFNGLTEXIMAGE2DPROC glad_glTexImage2D;
extern PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap;
extern PFNGLDELETETEXTURESPROC glad_glDeleteTextures;

extern PFNGLBLENDFUNCPROC glad_glBlendFunc;
extern PFNGLBLENDEQUATIONPROC glad_glBlendEquation;

extern PFNGLDRAWARRAYSINSTANCEDPROC glad_glDrawArraysInstanced;

extern PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus;
extern PFNGLDRAWBUFFERSPROC glad_glDrawBuffers;

// map to the usual opengl names
#define glGenVertexArrays glad_glGenVertexArrays
#define glBindVertexArray glad_glBindVertexArray
#define glDeleteVertexArrays glad_glDeleteVertexArrays

#define glGenBuffers glad_glGenBuffers
#define glBindBuffer glad_glBindBuffer
#define glBufferData glad_glBufferData
#define glBufferSubData glad_glBufferSubData
#define glDeleteBuffers glad_glDeleteBuffers

#define glEnableVertexAttribArray glad_glEnableVertexAttribArray
#define glVertexAttribPointer glad_glVertexAttribPointer
#define glVertexAttribDivisor glad_glVertexAttribDivisor

#define glCreateShader glad_glCreateShader
#define glShaderSource glad_glShaderSource
#define glCompileShader glad_glCompileShader
#define glGetShaderiv glad_glGetShaderiv
#define glGetShaderInfoLog glad_glGetShaderInfoLog

#define glCreateProgram glad_glCreateProgram
#define glAttachShader glad_glAttachShader
#define glLinkProgram glad_glLinkProgram
#define glGetProgramiv glad_glGetProgramiv
#define glGetProgramInfoLog glad_glGetProgramInfoLog
#define glUseProgram glad_glUseProgram
#define glDeleteShader glad_glDeleteShader
#define glDeleteProgram glad_glDeleteProgram

#define glGetUniformLocation glad_glGetUniformLocation
#define glUniformMatrix4fv glad_glUniformMatrix4fv
#define glUniform3fv glad_glUniform3fv
#define glUniform4fv glad_glUniform4fv
#define glUniform2fv glad_glUniform2fv
#define glUniform1f glad_glUniform1f
#define glUniform1i glad_glUniform1i

#define glActiveTexture glad_glActiveTexture
#define glGenTextures glad_glGenTextures
#define glBindTexture glad_glBindTexture
#define glTexParameteri glad_glTexParameteri
#define glTexImage2D glad_glTexImage2D
#define glGenerateMipmap glad_glGenerateMipmap
#define glDeleteTextures glad_glDeleteTextures

#define glBlendFunc glad_glBlendFunc
#define glBlendEquation glad_glBlendEquation

#define glDrawArraysInstanced glad_glDrawArraysInstanced

#define glGenFramebuffers glad_glGenFramebuffers
#define glBindFramebuffer glad_glBindFramebuffer
#define glDeleteFramebuffers glad_glDeleteFramebuffers
#define glFramebufferTexture2D glad_glFramebufferTexture2D
#define glCheckFramebufferStatus glad_glCheckFramebufferStatus
#define glDrawBuffers glad_glDrawBuffers

// loader entrypoint
int gladLoadGLLoader(GLADloadproc load);

#ifdef __cplusplus
}
#endif
