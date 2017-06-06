#ifndef __INPUT_WINDOW_HPP__
#define __INPUT_WINDOW_HPP__

#include <functional>
#include <vector>

#include "input_window_gl_state.hpp"
#include "signal_dispatcher.hpp"
#include "glfw.hpp"
#include "target_point.hpp"

struct input_window: public glfw::window {
    input_window(std::size_t width, std::size_t height,
                        signal_dispatcher const & signal_dispatcher);
    virtual ~input_window();
    void redraw(frame const & f, std::vector<target_point> const & pts) const;
private:
    virtual void cursor_pos_callback_impl(double xpos, double ypos);
    virtual void mouse_button_callback_impl(int button, int action, int mods);
    virtual void scroll_callback_impl(double xoffset, double yoffset);
    virtual void key_callback_impl(int key, int scancode, int action, int mods);

    input_window_gl_state gl_state_;

    double xpos_;
    double ypos_;
    
    std::function<void()> left_mouse_down_;
    std::function<void()> right_mouse_down_;
    std::function<void(double, double)> scroll_;
    std::function<void(int, int)> key_down_;
};

#endif // __CLIENT_INPUT_WINDOW_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
