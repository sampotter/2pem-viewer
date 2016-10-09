#include "client_input_window.hpp"

#include <boost/signals2.hpp>

#include <iostream>

client_input_window::client_input_window(
	std::size_t width,
	std::size_t height,
	client_signal_dispatcher const & signal_dispatcher):
	window(width, height),
	left_mouse_down_ {[&] () {
		signal_dispatcher.left_mouse_down(xpos_, ypos_);
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
												int mods)
{
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		left_mouse_down_();
		break;
	default:
		break;
	}
}

void
client_input_window::scroll_callback_impl(double xoffset, double yoffset)
{
}
