#ifndef __CLIENT_GLFW_STATE_HPP__
#define __CLIENT_GLFW_STATE_HPP__

#include "client_input_window.hpp"
#include "client_options.hpp"
#include "client_signal_dispatcher.hpp"
#include "client_slm_window.hpp"
#include "glfw.hpp"

struct client_glfw_state {
	client_glfw_state(client_options const & options,
					  client_signal_dispatcher & signal_dispatcher);
	void init();
	void cleanup() const;
	client_input_window const & get_input_window() const;
private:
	glfw::library library_;
	client_input_window input_window_;
	client_slm_window slm_window_;
};

#endif // __CLIENT_GLFW_STATE_HPP__
