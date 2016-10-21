#include "client_glfw_state.hpp"

#include <iostream>

client_glfw_state::client_glfw_state(
	client_options const & options,
	client_signal_dispatcher & signal_dispatcher):
	input_window_ {
	    options.get_img_width(),
		options.get_img_height(),
		signal_dispatcher
    },
	slm_window_ {
		options.get_slm_width(),
		options.get_slm_height(),
		signal_dispatcher
	}
{}

void
client_glfw_state::init()
{
	slm_window_.hide(); // TODO: make this work...
	input_window_.make_context_current();
}

void
client_glfw_state::cleanup() const
{
	glfw::terminate();
}

client_input_window const &
client_glfw_state::get_input_window() const
{
	return input_window_;
}
