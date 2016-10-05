#include "client_glfw_state.hpp"

void
client_glfw_state::init(client_options const & options)
{
	glfw::init();
	window_ = glfw::window(options.get_img_width(), options.get_img_height());
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
