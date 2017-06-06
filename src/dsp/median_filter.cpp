#include "median_filter.hpp"

void
median_filter::apply_impl(image const * const inputs, image * const outputs)
	const
{
	auto const & in = inputs[0];
	auto const & out = outputs[0];

	// TODO: need index iterator or pixel iterator...
}
