#ifndef __INPUT_WINDOW_GL_STATE_HPP__
#define __INPUT_WINDOW_GL_STATE_HPP__

#include "frame.hpp"
#include "gl.hpp"
#include "glfw.hpp"
#include "target_point.hpp"

// TODO: this currently lives in app_state, but really should
// be moved into client_input_window.

struct input_window_gl_state {
    void init() const;
    void init_texture();
    void init_pbo(std::size_t img_width, std::size_t img_height);
    void init_vertex_vbo();
    void init_texcoords_vbo();
    void init_circle_vbo();
    void init_shader(char const * source, GLenum shader_type, GLuint * shader);
    void init_scope_frame_vshader();
    void init_scope_frame_fshader();
    void init_scope_frame_shader_program();
    void init_target_circle_vshader();
    void init_target_circle_fshader();
    void init_target_circle_shader_program();
    void init_locations();
    void update_viewport(glfw::window const & window) const;
    void buffer_frame(frame const & f) const;
    void texture_frame(frame const & f) const;
    void draw_texture() const;
    void draw_target_circles(std::vector<target_point> const & pts) const;
    void cleanup();
private:
    GLuint texture_;
    GLuint pbo_;
    GLsizeiptr pbo_size_;
    GLuint verts_vbo_;
    GLuint texcoords_vbo_;
    GLuint circle_vbo_;
    GLuint scope_frame_vshader_;
    GLuint scope_frame_fshader_;
    GLuint scope_frame_shader_program_;
    GLuint target_circle_vshader_;
    GLuint target_circle_fshader_;
    GLuint target_circle_shader_program_;
    GLint a_coord2d_;
    GLint a_texcoord_;
    GLint u_tex_;
    GLint a_coord2d_target_circle_;
};

#endif // __INPUT_WINDOW_GL_STATE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
