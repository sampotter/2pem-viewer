#include "client_input_window.hpp"

#include <iostream>

void
client_input_window::cursor_pos_callback_impl(double xpos, double ypos)
{
	std::cout << xpos << ", " << ypos << std::endl;
}

void
client_input_window::mouse_button_callback_impl(int button, int action,
												int mods)
{
	std::cout << button << ", " << action << ", " << mods << std::endl;
}

void
client_input_window::scroll_callback_impl(double xoffset, double yoffset)
{
	std::cout << xoffset << ", " << yoffset << std::endl;
}
