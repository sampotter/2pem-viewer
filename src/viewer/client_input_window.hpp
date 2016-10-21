#ifndef __CLIENT_INPUT_WINDOW_HPP__
#define __CLIENT_INPUT_WINDOW_HPP__

#include <functional>

#include "client_signal_dispatcher.hpp"
#include "glfw.hpp"

struct client_input_window: public glfw::window {
	client_input_window(std::size_t width, std::size_t height,
						client_signal_dispatcher const & signal_dispatcher);
private:
	virtual void cursor_pos_callback_impl(double xpos, double ypos);
	virtual void mouse_button_callback_impl(int button, int action, int mods);
	virtual void scroll_callback_impl(double xoffset, double yoffset);
	virtual void key_callback_impl(int key, int scancode, int action, int mods);

	double xpos_;
	double ypos_;
	std::function<void()> left_mouse_down_;
	std::function<void()> right_mouse_down_;
	std::function<void(double, double)> scroll_;
	std::function<void(int, int)> key_down_;
};

#endif // __CLIENT_INPUT_WINDOW_HPP__
