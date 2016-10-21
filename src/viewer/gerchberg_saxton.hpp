#ifndef __GERCHBERG_SAXTON_HPP__
#define __GERCHBERG_SAXTON_HPP__

#include <cstddef>

#include "frame.hpp"

namespace gerchberg_saxton {
	void compute_phase_mask(
		double const * source,
		double const * target,
		std::size_t in_width,
		std::size_t in_height,
		std::size_t out_width,
		std::size_t out_height,
		std::size_t iter_count,
		frame & phase_mask);

	void apply_axicon_phase_mask(double axicon_radius, frame & phase_mask);
}

#endif // __GERCHBERG_SAXTON_HPP__
