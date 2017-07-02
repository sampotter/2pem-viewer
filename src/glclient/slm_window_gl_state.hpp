#ifndef __SLM_WINDOW_GL_STATE_HPP__
#define __SLM_WINDOW_GL_STATE_HPP__

#include <dsp/frame.hpp>

#include <glclient/gl.hpp>
#include <glclient/glfw.hpp>

struct slm_window_gl_state {
    // TODO: most of these duplicate functions in
    // input_window_gl_state and should be made into free
    // functions and put somewhere else.
    
    void init() const;
    void init_texture();
    void init_pbo(std::size_t slm_width, std::size_t slm_height);
    void init_vertex_vbo();
    void init_texcoords_vbo();
    void init_shader(char const * source, GLenum shader_type, GLuint * shader);
    void init_vshader();
    void init_fshader();
    void init_shader_program();
    void init_locations();
    void update_viewport(glfw::window const & window) const;
    void buffer_frame(frame const & f) const;
    void texture_frame(frame const & f) const;
    void draw_texture() const;
    void clear() const;
    void cleanup();
private:
    // TODO: this just replicates what's in the input_window_gl_state
    // struct... Need to figure out a good way to manage this.
    GLuint texture_;
    GLuint pbo_;
    GLsizeiptr pbo_size_;
    GLuint verts_vbo_;
    GLuint texcoords_vbo_;
    GLuint vshader_;
    GLuint fshader_;
    GLuint shader_program_;
    GLint a_coord2d_;
    GLint a_texcoord_;
    GLint u_tex_;
};

#endif // __SLM_WINDOW_GL_STATE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
