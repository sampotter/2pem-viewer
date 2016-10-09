#ifndef __CLIENT_GLFW_STATE_HPP__
#define __CLIENT_GLFW_STATE_HPP__

#include "client_options.hpp"
#include "glfw.hpp"

struct client_glfw_state {
	client_glfw_state(client_options const & options);
	void init();
	void cleanup() const;
	glfw::window const & get_window() const;
private:
	glfw::library library_;
	glfw::window window_;
};

#endif // __CLIENT_GLFW_STATE_HPP__
