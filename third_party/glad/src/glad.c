#include <glad/glad.h>
#include <stdio.h>

// function pointers
PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays = NULL;

PFNGLGENBUFFERSPROC glad_glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glad_glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glad_glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glad_glBufferSubData = NULL;
PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers = NULL;

PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer = NULL;
PFNGLVERTEXATTRIBDIVISORPROC glad_glVertexAttribDivisor = NULL;

PFNGLCREATESHADERPROC glad_glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glad_glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glad_glCompileShader = NULL;
PFNGLGETSHADERIVPROC glad_glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog = NULL;

PFNGLCREATEPROGRAMPROC glad_glCreateProgram = NULL;
PFNGLATTACHSHADERPROC glad_glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glad_glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glad_glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog = NULL;
PFNGLUSEPROGRAMPROC glad_glUseProgram = NULL;
PFNGLDELETESHADERPROC glad_glDeleteShader = NULL;
PFNGLDELETEPROGRAMPROC glad_glDeleteProgram = NULL;

PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation = NULL;
PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv = NULL;
PFNGLUNIFORM3FVPROC glad_glUniform3fv = NULL;
PFNGLUNIFORM4FVPROC glad_glUniform4fv = NULL;
PFNGLUNIFORM2FVPROC glad_glUniform2fv = NULL;
PFNGLUNIFORM1FPROC glad_glUniform1f = NULL;
PFNGLUNIFORM1IPROC glad_glUniform1i = NULL;

PFNGLACTIVETEXTUREPROC glad_glActiveTexture = NULL;
PFNGLGENTEXTURESPROC glad_glGenTextures = NULL;
PFNGLBINDTEXTUREPROC glad_glBindTexture = NULL;
PFNGLTEXPARAMETERIPROC glad_glTexParameteri = NULL;
PFNGLTEXIMAGE2DPROC glad_glTexImage2D = NULL;
PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap = NULL;
PFNGLDELETETEXTURESPROC glad_glDeleteTextures = NULL;

PFNGLBLENDFUNCPROC glad_glBlendFunc = NULL;
PFNGLBLENDEQUATIONPROC glad_glBlendEquation = NULL;

PFNGLDRAWARRAYSINSTANCEDPROC glad_glDrawArraysInstanced = NULL;

PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus = NULL;
PFNGLDRAWBUFFERSPROC glad_glDrawBuffers = NULL;

static void* glad_get(GLADloadproc load, const char* name) {
    void* p = load(name);
    return p;
}

static int glad_load_all(GLADloadproc load) {
    // vao
    glad_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glad_get(load, "glGenVertexArrays");
    glad_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glad_get(load, "glBindVertexArray");
    glad_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glad_get(load, "glDeleteVertexArrays");
    // vbo
    glad_glGenBuffers = (PFNGLGENBUFFERSPROC)glad_get(load, "glGenBuffers");
    glad_glBindBuffer = (PFNGLBINDBUFFERPROC)glad_get(load, "glBindBuffer");
    glad_glBufferData = (PFNGLBUFFERDATAPROC)glad_get(load, "glBufferData");
    glad_glBufferSubData = (PFNGLBUFFERSUBDATAPROC)glad_get(load, "glBufferSubData");
    glad_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glad_get(load, "glDeleteBuffers");
    // attribs
    glad_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glad_get(load, "glEnableVertexAttribArray");
    glad_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glad_get(load, "glVertexAttribPointer");
    glad_glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)glad_get(load, "glVertexAttribDivisor");
    // shaders
    glad_glCreateShader = (PFNGLCREATESHADERPROC)glad_get(load, "glCreateShader");
    glad_glShaderSource = (PFNGLSHADERSOURCEPROC)glad_get(load, "glShaderSource");
    glad_glCompileShader = (PFNGLCOMPILESHADERPROC)glad_get(load, "glCompileShader");
    glad_glGetShaderiv = (PFNGLGETSHADERIVPROC)glad_get(load, "glGetShaderiv");
    glad_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glad_get(load, "glGetShaderInfoLog");
    glad_glCreateProgram = (PFNGLCREATEPROGRAMPROC)glad_get(load, "glCreateProgram");
    glad_glAttachShader = (PFNGLATTACHSHADERPROC)glad_get(load, "glAttachShader");
    glad_glLinkProgram = (PFNGLLINKPROGRAMPROC)glad_get(load, "glLinkProgram");
    glad_glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glad_get(load, "glGetProgramiv");
    glad_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glad_get(load, "glGetProgramInfoLog");
    glad_glUseProgram = (PFNGLUSEPROGRAMPROC)glad_get(load, "glUseProgram");
    glad_glDeleteShader = (PFNGLDELETESHADERPROC)glad_get(load, "glDeleteShader");
    glad_glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glad_get(load, "glDeleteProgram");
    // uniforms
    glad_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glad_get(load, "glGetUniformLocation");
    glad_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glad_get(load, "glUniformMatrix4fv");
    glad_glUniform3fv = (PFNGLUNIFORM3FVPROC)glad_get(load, "glUniform3fv");
    glad_glUniform4fv = (PFNGLUNIFORM4FVPROC)glad_get(load, "glUniform4fv");
    glad_glUniform2fv = (PFNGLUNIFORM2FVPROC)glad_get(load, "glUniform2fv");
    glad_glUniform1f = (PFNGLUNIFORM1FPROC)glad_get(load, "glUniform1f");
    glad_glUniform1i = (PFNGLUNIFORM1IPROC)glad_get(load, "glUniform1i");
    // textures
    glad_glActiveTexture = (PFNGLACTIVETEXTUREPROC)glad_get(load, "glActiveTexture");
    glad_glGenTextures = (PFNGLGENTEXTURESPROC)glad_get(load, "glGenTextures");
    glad_glBindTexture = (PFNGLBINDTEXTUREPROC)glad_get(load, "glBindTexture");
    glad_glTexParameteri = (PFNGLTEXPARAMETERIPROC)glad_get(load, "glTexParameteri");
    glad_glTexImage2D = (PFNGLTEXIMAGE2DPROC)glad_get(load, "glTexImage2D");
    glad_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glad_get(load, "glGenerateMipmap");
    glad_glDeleteTextures = (PFNGLDELETETEXTURESPROC)glad_get(load, "glDeleteTextures");
    // blending
    glad_glBlendFunc = (PFNGLBLENDFUNCPROC)glad_get(load, "glBlendFunc");
    glad_glBlendEquation = (PFNGLBLENDEQUATIONPROC)glad_get(load, "glBlendEquation");
    // draw
    glad_glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)glad_get(load, "glDrawArraysInstanced");
    // fbo
    glad_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glad_get(load, "glGenFramebuffers");
    glad_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glad_get(load, "glBindFramebuffer");
    glad_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)glad_get(load, "glDeleteFramebuffers");
    glad_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glad_get(load, "glFramebufferTexture2D");
    glad_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glad_get(load, "glCheckFramebufferStatus");
    glad_glDrawBuffers = (PFNGLDRAWBUFFERSPROC)glad_get(load, "glDrawBuffers");

    // basic sanity
    return glad_glCreateShader && glad_glCreateProgram && glad_glGenVertexArrays
        && glad_glDrawArraysInstanced && glad_glGenFramebuffers;
}

int gladLoadGLLoader(GLADloadproc load) {
    if (!load) return 0;
    return glad_load_all(load);
}
