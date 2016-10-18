#ifndef __CLIENT_SIGNAL_DISPATCHER_HPP__
#define __CLIENT_SIGNAL_DISPATCHER_HPP__

#include <functional>

#include <boost/signals2.hpp>

struct client_signal_dispatcher {
	void left_mouse_down(double xpos, double ypos) const;
	void connect_left_mouse_down_slot(std::function<void(double, double)> & cb);
	void right_mouse_down(double xpos, double ypos) const;
	void connect_right_mouse_down_slot(std::function<void(double, double)> & cb);
	void scroll(double xoffset, double yoffset) const;
	void connect_scroll_slot(std::function<void(double, double)> & cb);
private:
	boost::signals2::signal<void(double, double)> left_mouse_down_signal_;
	boost::signals2::signal<void(double, double)> right_mouse_down_signal_;
	boost::signals2::signal<void(double, double)> scroll_signal_;
};

#endif // __CLIENT_SIGNAL_DISPATCHER_HPP__
