#ifndef __PHASE_RETRIEVAL_HPP__
#define __PHASE_RETRIEVAL_HPP__

#include <cstddef>

#include "frame.hpp"
#include "lens_parameters.hpp"

namespace phase_retrieval {
    void compute_phase_mask(
        double const * source,
        double const * target,
        std::size_t in_width,
        std::size_t in_height,
        slm_parameters const & slm_params,
        std::size_t iter_count,
        frame & phase_mask);

    void apply_axicon_phase_mask(double axicon_radius, frame & phase_mask);

    void apply_lens_function(
        slm_parameters const & slm_params,
        lens_parameters const & lens_params,
        frame & phase_mask);
}

#endif // __PHASE_RETRIEVAL_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
