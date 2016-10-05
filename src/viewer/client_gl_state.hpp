#ifndef __CLIENT_GL_STATE_HPP__
#define __CLIENT_GL_STATE_HPP__

#include "client_options.hpp"
#include "frame.hpp"
#include "gl.hpp"
#include "glfw.hpp"

struct client_gl_state {
	void init() const;
	void init_texture();
	void init_pbo(client_options const & options);
	void init_vertex_vbo();
	void init_texcoords_vbo();
	void init_shader(char const * source, GLenum shader_type, GLuint * shader);
	void init_vertex_shader();
	void init_fragment_shader();
	void init_shader_program();
	void init_locations();
	void update_viewport(glfw::window const & window) const;
	void buffer_frame(frame const & f) const;
	void texture_frame(frame const & f) const;
	void draw_texture() const;
	void cleanup();
private:
	GLuint texture_;
	GLuint pbo_;
	GLsizeiptr pbo_size_;
	GLuint verts_vbo_;
	GLuint texcoords_vbo_;
	GLuint vertex_shader_;
	GLuint fragment_shader_;
	GLuint program_;
	GLint a_coord2d_;
	GLint a_texcoord_;
	GLint u_tex_;
};

#endif // __CLIENT_GL_STATE_HPP__
