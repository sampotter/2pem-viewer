#ifndef __CLIENT_OSC_STATE_HPP__
#define __CLIENT_OSC_STATE_HPP__

#include <cstddef>
#include <memory>

struct client_osc_state {
	client_osc_state(std::size_t port);
	~client_osc_state();
private:
	struct impl;
	std::unique_ptr<impl> impl_;
};

#endif // __CLIENT_OSC_STATE_HPP__
