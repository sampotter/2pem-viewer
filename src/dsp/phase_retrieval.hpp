#ifndef __PHASE_RETRIEVAL_HPP__
#define __PHASE_RETRIEVAL_HPP__

#include <cstddef>

#include "frame.hpp"

namespace phase_retrieval {
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

    void apply_lens_function(
        double dim_x,
        double dim_y,
        double focal_length,
        double wavelength,
        frame & phase_mask);
}

#endif // __PHASE_RETRIEVAL_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
