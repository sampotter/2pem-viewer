#include "client_slm_window.hpp"

#include <iostream>

client_slm_window ::client_slm_window(
	std::size_t width,
	std::size_t height,
	client_signal_dispatcher & signal_dispatcher):
	window(width, height)
{
	{
		auto cb = std::function<void(frame const &)> {
			[this] (frame const & f) {
				redraw(f);
			}
		};
		signal_dispatcher.connect_redraw_slm_window_slot(cb);
	}
}

void
client_slm_window::cursor_pos_callback_impl(double /* xpos */,
											double /* ypos */)
{
}

void
client_slm_window::mouse_button_callback_impl(int /* button */,
											  int /* action */,
											  int /* mods */)
{
}

void
client_slm_window::scroll_callback_impl(double /* xoffset */,
										double /* yoffset */)
{
}

void
client_slm_window::key_callback_impl(int /* key */,
									 int /* scancode */,
									 int /* action */,
									 int /* mods */)
{
}

void
client_slm_window::redraw(frame const & /* f */) const
{
	std::cout << "redraw" << std::endl;
}
