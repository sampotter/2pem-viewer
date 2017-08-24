#include "phase_retrieval.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

#include <armadillo>

#include "math.hpp"

template <class T>
arma::Mat<T> fftshift(arma::Mat<T> const & X) {
    using namespace arma;
    auto m = X.n_rows;
    auto n = X.n_cols;
    Mat<T> Y = zeros<Mat<T>>(m, n);
    auto r = m/2, p = r + 1;
    auto s = n/2, q = s + 1;
    auto I1 = span(0, r), I2 = span(m - p, m - 1);
    auto J1 = span(0, s), J2 = span(n - q, n - 1);
    Y(I1, J1) = X(I2, J2);
    Y(I2, J2) = X(I1, J1);
    Y(I1, J2) = X(I2, J1);
    Y(I2, J1) = X(I1, J2);
    return Y;
}

template <class T>
arma::Mat<T> ifftshift(arma::Mat<T> const & X) {
    using namespace arma;
    auto m = X.n_rows;
    auto n = X.n_cols;
    auto Y = zeros<Mat<T>>(m, n);
    auto r = m/2, p = r + 1;
    auto s = n/2, q = s + 1;
    auto I1 = span(0, r), I2 = span(p, m - 1);
    auto J1 = span(0, s), J2 = span(q, n - 1);
    Y(I1, J1) = X(I2, J2);
    Y(I2, J2) = X(I1, J1);
    Y(I1, J2) = X(I2, J1);
    Y(I2, J1) = X(I1, J2);
    return Y;
}

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
    
    cx_mat S(in_height, in_width);
    cx_mat T(in_height, in_width);

    for (auto i {0ul}; i < in_height; ++i) {
        for (auto j {0ul}; j < in_width; ++j) {
            S(i, j) = source[in_width*i + j];
            T(i, j) = target[in_width*i + j];
        }
    }

    cx_mat A = ifft2(T);
    cx_mat B(in_height, in_width);
    cx_mat C(in_height, in_width);
    cx_mat D(in_height, in_width);

    decltype(iter_count) iter {2};
    while (iter <= iter_count) {
        B = S % exp(cx_double(0, 1)*arg(fftshift(A)));
        C = fft2(B);
        D = T % exp(cx_double(0, 1)*arg(C));
        A = ifft2(fftshift(D));
        ++iter;
    }

    mat A_arg = (arg(A) + datum::pi)/(2*datum::pi);

    auto tmp = phase_mask.data();
    auto k {0ul};
    for (auto i {0ul}; i < out_height; ++i) {
        for (auto j {0ul}; j < out_width; ++j) {
            tmp[k++] = A_arg(i % in_height, j % in_width);
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
    double x_dim,
    double y_dim,
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

    auto const pixel_width_mm = x_dim/static_cast<double>(w);
    auto const pixel_height_mm = y_dim/static_cast<double>(h);

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
