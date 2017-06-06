#ifndef __OSC_STATE_HPP__
#define __OSC_STATE_HPP__

#include <cstddef>
#include <memory>

struct osc_state {
	osc_state(std::size_t port);
	~osc_state();
private:
	struct impl;
	std::unique_ptr<impl> impl_;
};

#endif // __OSC_STATE_HPP__
