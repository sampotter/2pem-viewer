#include "phase_retrieval.hpp"

#include <cassert>
#include <cmath>
#include <type_traits>

#include <armadillo>

#include "math.hpp"

void
phase_retrieval::compute_phase_mask(
    double const * source,
    double const * target,
    std::size_t in_width,
    std::size_t in_height,
    slm_parameters const & slm_params,
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

    auto const w = slm_params.resolution.width;
    auto const h = slm_params.resolution.height;

    auto tmp = phase_mask.data();
    for (auto i {0ul}; i < h; ++i) {
        for (auto j {0ul}; j < w; ++j) {
            tmp[w*i + j] = A(i % in_height, j % in_width).real();
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

void
phase_retrieval::apply_lens_function(
    slm_parameters const & slm_params,
    double focal_length,
    double wavelength,
    frame & phase_mask)
{
    using namespace arma;
    
    auto const w = phase_mask.get_width();
    auto const h = phase_mask.get_height();

    auto const f = focal_length;
    auto const f_abs = std::fabs(f);
    auto const f_sign = sign(f);
    auto const f_sqr = f*f;

    // TODO: wavelength is in nanometers. We should probably use
    // boost::units for this at some point.
    auto const scaled_wavelength = wavelength*1e-6;

    auto const pixel_width_mm =
        slm_params.dimensions.width/static_cast<double>(w);
    auto const pixel_height_mm =
        slm_params.dimensions.height/static_cast<double>(h);

    auto const center_x = (w + 1)/2.0;
    auto const center_y = (h + 1)/2.0;

    std::remove_const<decltype(center_x)>::type x;
    decltype(x) y;

    auto data = phase_mask.data();
    std::remove_pointer<decltype(data)>::type lens_value;

    for (auto j {0ul}; j < w; ++j) {
        y = pixel_height_mm*(j - center_y);
        for (auto i {0ul}; i < h; ++i) {
            x = pixel_width_mm*(i - center_x);
            lens_value = f_sign*(sqrt(f_sqr + x*x + y*y) - f_abs)/
                scaled_wavelength;
            data[w*i + j] = std::fmod(data[w*i + j] + lens_value, 1.0);
        }
    }
}

// Local Variables:
// indent-tabs-mode: nil
// End:
