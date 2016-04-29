/* ////////////////////////////////////////////////////////////

File Name: ysglwindows.h
Copyright (c) 2015 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#ifndef COMMON_IS_INCLUDED
#define COMMON_IS_INCLUDED
/* { */

// Microsoft, concede and give up on Direct3D.  Nobody wants Direct 3D any more. Soji

/* Force Visual C++ to type-mismatching error. */
#pragma warning( error : 4028)
#pragma warning( error : 4047)

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
void glActiveTexture(GLenum texture);

void glPointParameterf(GLenum pname,GLfloat param);
void glPointParameteri(GLenum pname,GLint param);
void glPointParameterfv(GLenum pname,const GLfloat *params);
void glPointParameteriv(GLenum pname,const GLint *params);

GLboolean glIsBuffer(GLuint buffer);
void glGenBuffers(GLsizei n,const GLuint *buffers);
void glDeleteBuffers(GLsizei n,const GLuint *buffers);
void glBindBuffer(GLenum target,GLuint buffer);
void glBufferData(GLenum target,GLsizeiptr size,const GLvoid *data,GLenum usage);
void glBufferSubData(GLenum target,GLintptr offset,GLsizeiptr size,const GLvoid *data);

void glGenFramebuffers(GLsizei n,GLuint *framebuffers);
void glDeleteFramebuffers(GLsizei n,const GLuint *framebuffers);
void glBindFramebuffer(GLenum target,GLuint framebuffer);
void glFramebufferTexture2D(GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level);
GLenum glCheckFramebufferStatus(GLenum target);

GLuint glCreateShader(GLenum shaderType);
void glDeleteShader(GLuint  shader);
void glCompileShader(GLuint shader);
void glShaderSource(GLuint shader,GLsizei count,const GLchar **string,const GLint *length);
void glGetShaderiv(GLuint shader,GLenum pname,GLint *params);
void glGetShaderInfoLog(GLuint shader,GLsizei maxLength,GLsizei *length,GLchar *infoLog);
GLuint glCreateProgram(void);
void glDeleteProgram(GLuint program);
void glAttachShader(GLuint program,GLuint shader);
void glLinkProgram(GLuint program);
void glGetProgramiv(GLuint program,  GLenum pname,  GLint *params); 
void glGetProgramInfoLog(GLuint program,GLsizei maxLength,GLsizei *length,GLchar *infoLog);
void glUseProgram(GLuint program);
void glBindAttribLocation(GLuint program,GLuint index,const GLchar *name);

GLint glGetUniformLocation(GLuint program,const GLchar *name);
void glUniform1f(GLint location,GLfloat v0);
void glUniform2f(GLint location,GLfloat v0,GLfloat v1); 
void glUniform3f(GLint location,GLfloat v0,GLfloat v1,GLfloat v2); 
void glUniform4f(GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3); 
void glUniform1i(GLint location,GLint v0); 
void glUniform2i(GLint location,GLint v0,GLint v1); 
void glUniform3i(GLint location,GLint v0,GLint v1,GLint v2); 
void glUniform4i(GLint location,GLint v0,GLint v1,GLint v2,  GLint v3);
void glUniform1fv(GLint location,GLsizei count,const GLfloat *value);
void glUniform2fv(GLint location,GLsizei count,const GLfloat *value);
void glUniform3fv(GLint location,GLsizei count,const GLfloat *value);
void glUniform4fv(GLint location,GLsizei count,const GLfloat *value);
void glUniform1iv(GLint location,GLsizei count,const GLint *value);
void glUniform2iv(GLint location,GLsizei count,const GLint *value);
void glUniform3iv(GLint location,GLsizei count,const GLint *value);
void glUniform4iv(GLint location,GLsizei count,const GLint *value);

void glUniformMatrix2fv(GLint  location,  GLsizei  count,  GLboolean  transpose,  const GLfloat * value);
void glUniformMatrix3fv(GLint  location,  GLsizei  count,  GLboolean  transpose,  const GLfloat * value);
void glUniformMatrix4fv(GLint  location,  GLsizei  count,  GLboolean  transpose,  const GLfloat * value);
void glUniformMatrix2x3fv(GLint  location,  GLsizei  count,  GLboolean  transpose,  const GLfloat * value);
void glUniformMatrix3x2fv(GLint  location,  GLsizei  count,  GLboolean  transpose,  const GLfloat * value);
void glUniformMatrix2x4fv(GLint  location,  GLsizei  count,  GLboolean  transpose,  const GLfloat * value);
void glUniformMatrix4x2fv(GLint  location,  GLsizei  count,  GLboolean  transpose,  const GLfloat * value);
void glUniformMatrix3x4fv(GLint  location,  GLsizei  count,  GLboolean  transpose,  const GLfloat * value);
void glUniformMatrix4x3fv(GLint  location,  GLsizei  count,  GLboolean  transpose,  const GLfloat * value);

GLint glGetAttribLocation(GLuint program,const GLchar *name);
void glEnableVertexAttribArray(GLuint index);
void glDisableVertexAttribArray(GLuint index);
void glVertexAttrib1f(GLuint index, GLfloat v0);
void glVertexAttrib1s(GLuint index, GLshort v0);
void glVertexAttrib1d(GLuint index, GLdouble v0);
void glVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1);
void glVertexAttrib2s(GLuint index, GLshort v0, GLshort v1);
void glVertexAttrib2d(GLuint index, GLdouble v0, GLdouble v1);
void glVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2);
void glVertexAttrib3s(GLuint index, GLshort v0, GLshort v1, GLshort v2);
void glVertexAttrib3d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2);
void glVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void glVertexAttrib4s(GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3);
void glVertexAttrib4d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
void glVertexAttrib4Nub(GLuint index, GLubyte v0, GLubyte v1, GLubyte v2, GLubyte v3);
void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);

void glVertexAttrib2fv(GLuint index,const GLfloat v[2]);
void glVertexAttrib3fv(GLuint index,const GLfloat v[3]);
void glVertexAttrib4fv(GLuint index,const GLfloat v[4]);

#endif

void InspectOpenGLVersionAndExtension(void);

#ifdef __cplusplus
} // Closing extern "C"
#endif

/* } */
#endif
