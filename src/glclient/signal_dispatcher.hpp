#ifndef __SIGNAL_DISPATCHER_HPP__
#define __SIGNAL_DISPATCHER_HPP__

#include <functional>

#include <dsp/frame.hpp>

struct signal_dispatcher {
    signal_dispatcher();
    ~signal_dispatcher();
    void left_mouse_down(double xpos, double ypos) const;
    void connect_left_mouse_down_slot(std::function<void(double, double)> & cb);
    void right_mouse_down(double xpos, double ypos) const;
    void connect_right_mouse_down_slot(std::function<void(double, double)> & cb);
    void scroll(double xoffset, double yoffset) const;
    void connect_scroll_slot(std::function<void(double, double)> & cb);
    void key_down(int key, int mods) const;
    void connect_key_down_slot(std::function<void(int, int)> & cb);
    void redraw_slm_window(frame const & f) const;
    void connect_redraw_slm_window_slot(std::function<void(frame const &)> & cb);
private:
    struct impl;
    impl * m_;
};

#endif // __SIGNAL_DISPATCHER_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
