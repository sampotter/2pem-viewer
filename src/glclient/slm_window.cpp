#include "client_slm_window.hpp"

#include <iostream>

client_slm_window::client_slm_window(std::size_t width, std::size_t height):
	window {width, height},
    phase_mask_ {width, height}
{
    make_context_current();
    gl_state_.init();
    gl_state_.init_texture();
    gl_state_.init_pbo(width, height);
    gl_state_.init_vertex_vbo();
    gl_state_.init_texcoords_vbo();
    gl_state_.init_vshader();
    gl_state_.init_fshader();
    gl_state_.init_shader_program();
    gl_state_.init_locations();
}

void
client_slm_window::clear() const
{
    make_context_current();
    gl_state_.clear();
    swap_buffers();
}

void
client_slm_window::redraw() const
{
    make_context_current();
    gl_state_.update_viewport(*this);
    gl_state_.buffer_frame(phase_mask_);
    gl_state_.texture_frame(phase_mask_);
    gl_state_.draw_texture();
    swap_buffers();
}

void
client_slm_window::set_phase_mask(frame const & phase_mask)
{
    phase_mask_ = phase_mask;
}

client_slm_window::~client_slm_window()
{
    make_context_current();
    gl_state_.cleanup();
}

void
client_slm_window::cursor_pos_callback_impl(double /* xpos */,
                                            double /* ypos */)
{
}

void
client_slm_window::mouse_button_callback_impl(int /* button */,
                                              int /* action */,
                                              int /* mods */)
{
}

void
client_slm_window::scroll_callback_impl(double /* xoffset */,
                                        double /* yoffset */)
{
}

void
client_slm_window::key_callback_impl(int /* key */,
                                     int /* scancode */,
                                     int /* action */,
                                     int /* mods */)
{
}

// Local Variables:
// indent-tabs-mode: nil
// End:
