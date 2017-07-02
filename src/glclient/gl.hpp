#ifndef __VIEWER_GL_HPP__
#define __VIEWER_GL_HPP__

#include <glclient/glheaders.hpp>

namespace gl {
    void activeTexture(GLenum texture);
    void attachShader(GLuint program, GLuint shader);
    void bindBuffer(GLenum target, GLuint buffer);
    void bindTexture(GLenum target, GLuint texture);
    void bufferData(GLenum target, GLsizeiptr size, GLvoid const * data,
                    GLenum usage);
    void bufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
                       GLvoid const * data);
    void clear(GLbitfield mask);
    void clearColor(GLclampf red, GLclampf green, GLclampf blue,
                    GLclampf alpha);
    void compileShader(GLuint shader);
    GLuint createProgram();
    GLuint createShader(GLenum shaderType);
    void deleteBuffers(GLsizei n, GLuint * buffers);
    void deleteProgram(GLuint program);
    void deleteShader(GLuint shader);
    void deleteTextures(GLsizei n, GLuint * textures);
    void disable(GLenum cap);
    void drawArrays(GLenum mode, GLint first, GLsizei count);
    void enableVertexAttribArray(GLint location);
    void flush();
    void genBuffers(GLsizei n, GLuint * buffers);
    void genTextures(GLsizei n, GLuint * textures);
    GLint getAttribLocation(GLuint program, GLchar const * name);
    void getProgramiv(GLuint program, GLenum pname, GLint * params);
    void getShaderiv(GLuint shader, GLenum pname, GLint * params);
    void getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei * length,
                          GLchar * infoLog);
    GLint getUniformLocation(GLuint program, GLchar const * name);
    void linkProgram(GLuint program);
    void shaderSource(GLuint shader, GLsizei count, GLchar const ** string,
                      GLint const * length);
    void texImage2D(GLenum target, GLint level, GLint internalformat,
                    GLsizei width, GLsizei height, GLint border, GLenum format,
                    GLenum type, GLvoid const * pixels);
    void texParameter(GLenum target, GLenum pname, GLint param);
    void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,
                       GLsizei width, GLsizei height, GLenum format, GLenum type,
                       GLvoid const * pixels);
    void uniform(GLint location, GLint v0);
    void useProgram(GLuint program);
    void vertexAttribPointer(GLint location, GLint size, GLenum type,
                             GLboolean normalized, GLsizei stride,
                             GLvoid const * pointer);
    void viewport(GLint x, GLint y, GLsizei width, GLsizei height);
}

#endif // __VIEWER_GL_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
