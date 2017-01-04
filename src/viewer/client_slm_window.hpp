#ifndef __CLIENT_SLM_WINDOW_HPP__
#define __CLIENT_SLM_WINDOW_HPP__

#include <functional>

#include "client_options.hpp"
#include "client_slm_window_gl_state.hpp"
#include "frame.hpp"
#include "glfw.hpp"

struct client_slm_window: public glfw::window {
    client_slm_window(std::size_t width, std::size_t height);
    virtual ~client_slm_window();
    void redraw(frame const & f) const;
    void clear() const;
private:
    virtual void cursor_pos_callback_impl(double xpos, double ypos);
    virtual void mouse_button_callback_impl(int button, int action, int mods);
    virtual void scroll_callback_impl(double xoffset, double yoffset);
    virtual void key_callback_impl(int key, int scancode, int action, int mods);

    client_slm_window_gl_state gl_state_;
};

#endif // __CLIENT_SLM_WINDOW_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
