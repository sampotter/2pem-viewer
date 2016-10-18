#include "client_signal_dispatcher.hpp"

void
client_signal_dispatcher::left_mouse_down(double xpos, double ypos) const
{
	left_mouse_down_signal_(xpos, ypos);
}

void
client_signal_dispatcher::connect_left_mouse_down_slot(
	std::function<void(double, double)> & cb)
{
	left_mouse_down_signal_.connect(cb);
}

void
client_signal_dispatcher::right_mouse_down(double xpos, double ypos) const
{
	right_mouse_down_signal_(xpos, ypos);
}

void
client_signal_dispatcher::connect_right_mouse_down_slot(
	std::function<void(double, double)> & cb)
{
	right_mouse_down_signal_.connect(cb);
}

void
client_signal_dispatcher::scroll(double xoffset, double yoffset) const
{
	scroll_signal_(xoffset, yoffset);
}

void
client_signal_dispatcher::connect_scroll_slot(
	std::function<void(double, double)> & cb)
{
	scroll_signal_.connect(cb);
}
