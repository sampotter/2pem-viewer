#ifndef __CLIENT_SLM_STATE_HPP__
#define __CLIENT_SLM_STATE_HPP__

#include "client_signal_dispatcher.hpp"
#include "target_point_store.hpp"

struct client_slm_state {
	client_slm_state(client_signal_dispatcher & signal_dispatcher);
	std::vector<target_point> const & get_target_points() const;
private:
	target_point_store point_store_;
};

#endif // __CLIENT_SLM_STATE_HPP__
