#ifndef __SLM_WINDOW_HPP__
#define __SLM_WINDOW_HPP__

#include <functional>

#include <dsp/frame.hpp>

#include "options.hpp"
#include "slm_window_gl_state.hpp"
#include "glfw.hpp"

struct slm_window: public glfw::window {
    slm_window(std::size_t width, std::size_t height);
    virtual ~slm_window();
    void clear() const;
    void redraw() const;
    void set_phase_mask(frame const & phase_mask);
private:
    virtual void cursor_pos_callback_impl(double xpos, double ypos);
    virtual void mouse_button_callback_impl(int button, int action, int mods);
    virtual void scroll_callback_impl(double xoffset, double yoffset);
    virtual void key_callback_impl(int key, int scancode, int action, int mods);

    slm_window_gl_state gl_state_;
    frame phase_mask_;
};

#endif // __SLM_WINDOW_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
