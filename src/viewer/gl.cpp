#include "gl.hpp"

#include <string>
#include <unordered_map>

#ifdef VIEWER_DEBUG
std::string
error_flag_to_string(GLenum error)
{
	switch (error) {
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
	default:
		throw std::runtime_error("Invalid OpenGL error flag");
	}
}
#endif // VIEWER_DEBUG

void handle_errors() {
#ifdef VIEWER_DEBUG
	auto const error = glGetError();
	if (error) {
		auto const error_string = error_flag_to_string(error);
		throw std::runtime_error(error_string);
	}
#endif // VIEWER_DEBUG
}

#ifdef VIEWER_DEBUG
/**
 * This map is lazily filled with associations between OpenGL
 * capabilities and whether or not they are enabled---i.e., when a
 * call to glEnable or glDisable is made, the capability and whether
 * or not it was enabled or disabled is recorded so that this
 * information can be queried easily for debugging.
 */
std::unordered_map<GLenum, bool> capability_map_;
#endif // VIEWER_DEBUG

void
gl::activeTexture(GLenum texture)
{
	glActiveTexture(texture);
	handle_errors();
}

void
gl::attachShader(GLuint program, GLuint shader)
{
	glAttachShader(program, shader);
	handle_errors();
}

void
gl::bindBuffer(GLenum target, GLuint buffer)
{
	glBindBuffer(target, buffer);
	handle_errors();
}

void
gl::bindTexture(GLenum target, GLuint texture)
{
	glBindTexture(target, texture);
	handle_errors();
}

void
gl::bufferData(GLenum target, GLsizeiptr size, GLvoid const * data,
			   GLenum usage)
{
	glBufferData(target, size, data, usage);
	handle_errors();
}

void
gl::bufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
				  GLvoid const * data)
{
	glBufferSubData(target, offset, size, data);
	handle_errors();
}

void
gl::clear(GLbitfield mask)
{
	glClear(mask);
	handle_errors();
}

void
gl::clearColor(GLclampf red, GLclampf green, GLclampf blue,	GLclampf alpha)
{
	glClearColor(red, green, blue, alpha);
	handle_errors();
}

void
gl::compileShader(GLuint shader)
{
	glCompileShader(shader);
	handle_errors();
}

GLuint
gl::createProgram()
{
	auto const program = glCreateProgram();
	handle_errors();
	return program;
}

GLuint
gl::createShader(GLenum shaderType)
{
	auto const shader = glCreateShader(shaderType);
	handle_errors();
	return shader;
}

void
gl::drawArrays(GLenum mode, GLint first, GLsizei count)
{
	glDrawArrays(mode, first, count);
	handle_errors();
}

void
gl::deleteTextures(GLsizei n, GLuint * textures)
{
	glDeleteTextures(n, textures);
	handle_errors();
}

void
gl::disable(GLenum cap)
{
	glDisable(cap);
	handle_errors();
#ifdef VIEWER_DEBUG
	capability_map_[cap] = false;
#endif // VIEWER_DEBUG
}

void
gl::enableVertexAttribArray(GLint location)
{
	glEnableVertexAttribArray(location);
	handle_errors();
}

void
gl::flush()
{
	glFlush();
	handle_errors();
}

void
gl::genBuffers(GLsizei n, GLuint * buffers)
{
	glGenBuffers(n, buffers);
	handle_errors();
}

void
gl::genTextures(GLsizei n, GLuint * textures)
{
	glGenTextures(n, textures);
	handle_errors();
}

GLint
gl::getAttribLocation(GLuint program, GLchar const * name)
{
	auto const attribLocation = glGetAttribLocation(program, name);
	handle_errors();
	return attribLocation;
}

void
gl::getProgramiv(GLuint program, GLenum pname, GLint * params)
{
	glGetProgramiv(program, pname, params);
	handle_errors();
}

void
gl::getShaderiv(GLuint shader, GLenum pname, GLint * params)
{
	glGetShaderiv(shader, pname, params);
	handle_errors();
}

void
gl::getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei * length,
					 GLchar *infoLog)
{
	glGetShaderInfoLog(shader, maxLength, length, infoLog);
	handle_errors();
}

GLint
gl::getUniformLocation(GLuint program, GLchar const * name)
{
	auto const uniformLocation = glGetUniformLocation(program, name);
	handle_errors();
	return uniformLocation;
}

void
gl::linkProgram(GLuint program)
{
	glLinkProgram(program);
	handle_errors();
}

void
gl::shaderSource(GLuint shader, GLsizei count, GLchar const ** string,
				 GLint const * length)
{
	glShaderSource(shader, count, string, length);
	handle_errors();
}

void
gl::texImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width,
			   GLsizei height, GLint border, GLenum format, GLenum type,
			   GLvoid const * pixels)
{
	glTexImage2D(target, level, internalformat, width, height, border, format,
				 type, pixels);
	handle_errors();
}

void
gl::texParameter(GLenum target, GLenum pname, GLint param)
{
	glTexParameteri(target, pname, param);
	handle_errors();
}

void
gl::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,
				  GLsizei width, GLsizei height, GLenum format, GLenum type,
				  GLvoid const * pixels)
{
	glTexSubImage2D(target, level, xoffset, yoffset, width, height, format,
					type, pixels);
	handle_errors();
}

void
gl::uniform(GLint location, GLint v0)
{
	glUniform1i(location, v0);
	handle_errors();
}

void
gl::useProgram(GLuint program)
{
	glUseProgram(program);
	handle_errors();
}

void
gl::vertexAttribPointer(GLint location, GLint size, GLenum type,
					   GLboolean normalized, GLsizei stride,
					   GLvoid const * pointer)
{
	glVertexAttribPointer(location, size, type, normalized, stride, pointer);
	handle_errors();
}

void
gl::viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	glViewport(x, y, width, height);
	handle_errors();
}
