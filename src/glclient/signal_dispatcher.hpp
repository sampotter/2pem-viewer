#ifndef __SIGNAL_DISPATCHER_HPP__
#define __SIGNAL_DISPATCHER_HPP__

#include <functional>

#include <boost/signals2.hpp>

#include "frame.hpp"

struct signal_dispatcher {
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
    boost::signals2::signal<void(double, double)> left_mouse_down_signal_;
    boost::signals2::signal<void(double, double)> right_mouse_down_signal_;
    boost::signals2::signal<void(double, double)> scroll_signal_;
    boost::signals2::signal<void(int, int)> key_down_signal_;
    boost::signals2::signal<void(frame const &)> redraw_slm_window_signal_;
};

#endif // __SIGNAL_DISPATCHER_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
