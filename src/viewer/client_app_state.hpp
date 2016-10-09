#ifndef __CLIENT_APP_STATE_HPP__
#define __CLIENT_APP_STATE_HPP__

#include <boost/optional.hpp>

#include "client_asio_state.hpp"
#include "client_error.hpp"
#include "client_gl_state.hpp"
#include "client_glfw_state.hpp"
#include "client_options.hpp"
#include "frame.hpp"

struct client_app_state {
	static client_app_state from_cli_args(int argc, char ** argv);
	~client_app_state();
	void init();
	void process_frame(client_error & error);
	void run();
private:
	client_app_state(client_options const & options);
	
	client_options options_;
	client_asio_state asio_state_;
	client_gl_state gl_state_;
	client_glfw_state glfw_state_;
	frame frame_;
	boost::optional<frame> template_frame_;
};

#endif // __CLIENT_APP_STATE_HPP__
