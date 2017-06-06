#include "input_window.hpp"

#include <boost/signals2.hpp>

#include <cmath>
#include <iostream>

input_window::input_window(
    std::size_t width,
    std::size_t height,
    signal_dispatcher const & signal_dispatcher):
    window(width, height),
    left_mouse_down_ {[&] () {
        signal_dispatcher.left_mouse_down(xpos_, ypos_);
    }},
    right_mouse_down_ {[&] () {
        signal_dispatcher.right_mouse_down(xpos_, ypos_);
    }},
    scroll_ {[&] (double xoffset, double yoffset) {
        signal_dispatcher.scroll(xoffset, yoffset);
    }},
    key_down_ {[&] (int key, int mods) {
        signal_dispatcher.key_down(key, mods);
    }}
{
    make_context_current();
    gl_state_.init();
    gl_state_.init_texture();
    gl_state_.init_pbo(width, height);
    gl_state_.init_vertex_vbo();
    gl_state_.init_texcoords_vbo();
    gl_state_.init_circle_vbo();
    gl_state_.init_scope_frame_vshader();
    gl_state_.init_scope_frame_fshader();
    gl_state_.init_scope_frame_shader_program();
    gl_state_.init_target_circle_vshader();
    gl_state_.init_target_circle_fshader();
    gl_state_.init_target_circle_shader_program();
    gl_state_.init_locations();
}

input_window::~input_window()
{
    make_context_current();
    gl_state_.cleanup();
}

void
input_window::redraw(frame const & f,
                     std::vector<target_point> const & pts) const
{
    make_context_current();
    gl_state_.update_viewport(*this);
    gl_state_.buffer_frame(f);
    gl_state_.texture_frame(f);
    gl_state_.draw_texture();
    gl_state_.draw_target_circles(pts);
    swap_buffers();
}

void
input_window::cursor_pos_callback_impl(double xpos, double ypos)
{
    xpos_ = xpos;
    ypos_ = ypos;
}

void
input_window::mouse_button_callback_impl(int button, int action,
                                         int /* mods */)
{
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        switch (action) {
        case GLFW_PRESS:
            left_mouse_down_();
            break;
        default:
            break;
        }
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        switch (action) {
        case GLFW_PRESS:
            right_mouse_down_();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void
input_window::scroll_callback_impl(double xoffset, double yoffset)
{
    scroll_(xoffset, yoffset);
}

void
input_window::key_callback_impl(int key, int /* scancode */, int action,
                                int mods)
{
    switch (action) {
    case GLFW_PRESS:
        key_down_(key, mods);
        break;
    default:
        break;
    }
}

// Local Variables:
// indent-tabs-mode: nil
// End:
