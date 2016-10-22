#include "phase_retrieval.hpp"

#include <cassert>
#include <cmath>

#include <armadillo>

#include "glfw.hpp"

void
phase_retrieval::compute_phase_mask(
    double const * source,
    double const * target,
    std::size_t in_width,
    std::size_t in_height,
    std::size_t out_width,
    std::size_t out_height,
    std::size_t iter_count,
    frame & phase_mask)
{
    using namespace arma;
    
    mat S(in_height, in_width);
    mat T(in_height, in_width);

    for (auto i {0ul}; i < in_height; ++i) {
        for (auto j {0ul}; j < in_width; ++j) {
            S(i, j) = source[in_width*i + j];
            T(i, j) = target[in_width*i + j];
        }
    }

    cx_mat A(in_height, in_width);
    cx_mat B(in_height, in_width);
    cx_mat C(in_height, in_width);
    cx_mat D(in_height, in_width);

    // TODO: very inefficient implementation for now, just to get
    // things working.
    
    auto const atan2 = [] (double y, double x) {
        if (x > 0) {
            return atan(y/x);
        } else if (x == 0) {
            if (y == 0) {
                return 0.0;
            } else {
                return y > 0 ? datum::pi/2 : -datum::pi/2;
            }
        } else {
            return atan(y/x) + (y >= 0 ? datum::pi : -datum::pi);
        }
    };

    auto const compute_angle = [&atan2] (cx_mat const & in, cx_mat & out) {
        assert(in.size() == out.size());
        for (auto i {0ul}; i < in.n_rows; ++i) {
            for (auto j {0ul}; j < in.n_cols; ++j) {
                out(i, j) = atan2(in(i, j).imag(), in(i, j).real());
            }
        }
    };

    A = atan(ifft2(A));
    decltype(iter_count) iter {2};
    while (iter <= iter_count) {
        B = S % exp(cx_double(0, 1)*A);
        compute_angle(fft2(B), C);
        D = T % exp(cx_double(0, 1)*C);
        compute_angle(ifft2(D), A);
        ++iter;
    }

    A = (A + datum::pi)/(2*datum::pi);

    auto tmp = phase_mask.data();
    for (auto i {0ul}; i < out_height; ++i) {
        for (auto j {0ul}; j < out_width; ++j) {
            tmp[out_width*i + j] = A(i % in_height, j % in_width).real();
        }
    }
}

void
phase_retrieval::apply_axicon_phase_mask(double axicon_radius, frame & mask)
{
    auto const w = mask.get_width();
    auto const h = mask.get_height();

    double x, y, z;

    auto tmp = mask.data();

    for (auto i {0ul}; i < h; ++i) {
        for (auto j {0ul}; j < w; ++j) {
            x = j - w/2.0;
            y = i - h/2.0;
            z = std::sqrt(x*x + y*y)/axicon_radius;
            
            tmp[w*i + j] = std::fmod(tmp[w*i + j] + z, 1.0);
        }
    }
}

// Local Variables:
// indent-tabs-mode: nil
// End:
