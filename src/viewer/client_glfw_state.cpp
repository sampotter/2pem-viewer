#include "client_glfw_state.hpp"

client_glfw_state::client_glfw_state(client_options const & options):
	window_ {options.get_img_width(), options.get_img_height()}
{}

void
client_glfw_state::init()
{
	window_.makeContextCurrent();
}

void
client_glfw_state::cleanup() const
{
	glfw::terminate();
}

glfw::window const &
client_glfw_state::get_window() const
{
	return window_;
}
