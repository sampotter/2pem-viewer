#include "binomial_filter.hpp"

void binomial_filter::apply_impl(image const * const inputs,
								 image * const outputs) const {
    image const & in = inputs[0];
    image & out = outputs[0];
    for (size_t i = 0; i < _size; ++i) {
        out[i] = 
    }
}

// Local Variables:
// indent-tabs-mode: nil
// End:
