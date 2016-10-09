#include "client_glfw_state.hpp"

client_glfw_state::client_glfw_state(client_options const & options):
	input_window_ {options.get_img_width(), options.get_img_height()},
	slm_window_ {options.get_slm_width(), options.get_slm_height()}
{}

void
client_glfw_state::init()
{
	input_window_.make_context_current();
}

void
client_glfw_state::cleanup() const
{
	glfw::terminate();
}

glfw::window const &
client_glfw_state::get_input_window() const
{
	return input_window_;
}
