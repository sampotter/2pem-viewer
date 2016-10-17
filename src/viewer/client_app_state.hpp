#ifndef __CLIENT_APP_STATE_HPP__
#define __CLIENT_APP_STATE_HPP__

#include <boost/optional.hpp>

#include "client_asio_state.hpp"
#include "client_error.hpp"
#include "client_gl_state.hpp"
#include "client_glfw_state.hpp"
#include "client_options.hpp"
#include "client_signal_dispatcher.hpp"
#include "client_slm_state.hpp"
#include "frame.hpp"

struct client_app_state {
	static client_app_state from_cli_args(int argc, char ** argv);
	~client_app_state();
	void init();
	void run();
private:
	client_app_state(client_options const & options);
	void process_frame(client_error & error);
	void draw_target_circles() const;
	void finish() const;

	client_options options_;
	client_signal_dispatcher signal_dispatcher_;
	client_asio_state asio_state_;
	client_gl_state gl_state_;
	client_glfw_state glfw_state_;
	client_slm_state slm_state_;
	frame frame_;
	boost::optional<frame> template_frame_;
	bool initialized_ {false};
};

#endif // __CLIENT_APP_STATE_HPP__
