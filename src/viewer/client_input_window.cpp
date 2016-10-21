#include "client_input_window.hpp"

#include <boost/signals2.hpp>

#include <cmath>
#include <iostream>

client_input_window::client_input_window(
	std::size_t width,
	std::size_t height,
	client_signal_dispatcher const & signal_dispatcher):
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
{}

void
client_input_window::cursor_pos_callback_impl(double xpos, double ypos)
{
	xpos_ = xpos;
	ypos_ = ypos;
}

void
client_input_window::mouse_button_callback_impl(int button, int action,
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
client_input_window::scroll_callback_impl(double xoffset, double yoffset)
{
	scroll_(xoffset, yoffset);
}

void
client_input_window::key_callback_impl(int key, int /* scancode */, int action,
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
