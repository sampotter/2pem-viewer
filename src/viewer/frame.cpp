#include "frame.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <fftw3.h>

frame::frame() {}

frame::frame(slm_parameters const & slm_params):
    data_(slm_params.resolution.width*slm_params.resolution.height),
    width_ {slm_params.resolution.width},
    height_ {slm_params.resolution.height}
{}

frame::frame(std::size_t width, std::size_t height):
    data_(width*height),
    width_ {width},
    height_ {height}
{}

void
frame::translate(int ishift, int jshift)
{
    std::size_t M = height_;
    std::size_t N = width_;

    int i_src_start = std::max(0, -ishift);
    int i_src_end = M - std::max(0, ishift);

    int i_dst_start = std::max(0, ishift);
    int i_dst_end = M - std::max(0, -ishift);

    int i_zero_start = ishift >= 0 ? 0 : M + ishift;
    int i_zero_end = ishift >= 0 ? ishift : M;

    memmove(&data_[i_dst_start*N], &data_[i_src_start*N],
            sizeof(GLfloat)*(i_src_end - i_src_start)*N);
    memset(&data_[i_zero_start*N], 0x0,
           sizeof(GLfloat)*(i_zero_end - i_zero_start)*N);

    int j_src_start = std::max(0, -jshift);
    int j_src_end = M - std::max(0, jshift);

    int j_dst_start = std::max(0, jshift);
    // int j_dst_end = M - std::max(0, -jshift);

    int j_zero_start = jshift >= 0 ? 0 : M + jshift;
    int j_zero_end = jshift >= 0 ? jshift : M;

    for (int i = i_dst_start; i < i_dst_end; ++i) {
        memmove(&data_[i*M + j_dst_start],
                &data_[i*M + j_src_start],
                sizeof(GLfloat)*(j_src_end - j_src_start));
        memset(&data_[i*M + j_zero_start], 0x0,
               sizeof(GLfloat)*(j_zero_end - j_zero_start));
    }
}

void
frame::align(frame const & other_frame)
{
    auto const n0 = width_;
    auto const n1 = height_;
    auto const N = n0*n1;
    auto const in_size = sizeof(double)*N;
    auto const out_size = sizeof(fftw_complex)*N;

    auto in = static_cast<double *>(fftw_malloc(in_size));
    for (std::size_t i {0}; i < N; ++i) {
        in[i] = data_[i];
    }
    
    auto out1 = static_cast<fftw_complex *>(fftw_malloc(out_size));
    auto out2 = static_cast<fftw_complex *>(fftw_malloc(out_size));

    auto plan = fftw_plan_dft_r2c_2d(n0, n1, in, out1, FFTW_ESTIMATE);
    fftw_execute(plan);

    for (std::size_t i {0}; i < N; ++i) {
        in[i] = other_frame.data_[i];
    }

    plan = fftw_plan_dft_r2c_2d(n0, n1, in, out2, FFTW_ESTIMATE);
    fftw_execute(plan);

    for (std::size_t i {0}; i < N; ++i) {
        out2[i][0] *= out1[i][0];
        out2[i][1] *= -out1[i][1];
    }

    plan = fftw_plan_dft_2d(n0, n1, out2, out1, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    int i0 = 0;
    int j0 = 0;
    std::size_t lindex;
    double re, im, mag, max_mag = -1;
    for (std::size_t i {0}; i < n0; ++i) {
        for (std::size_t j {0}; j < n1; ++j) {
            lindex = i*n1 + j;
            re = out1[lindex][0];
            im = out1[lindex][1];
            mag = std::sqrt(re*re + im*im);
            if (mag > max_mag) {
                max_mag = mag;
                i0 = i;
                j0 = j;
            }
        }
    }

#ifdef VIEWER_DEBUG
    assert(height_/2 <= std::numeric_limits<int>::max());
    assert(width_/2 <= std::numeric_limits<int>::max());
#endif // VIEWER_DEBUG
    if (i0 > static_cast<int>(height_/2)) {
        i0 -= height_;
    }
    if (j0 > static_cast<int>(width_/2)) {
        j0 -= width_;
    }

    translate(i0, j0);

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out1);
    fftw_free(out2);
    
#ifdef VIEWER_DEBUG
    fprintf(stdout, "%d, %d\n", i0, j0);
#endif // VIEWER_DEBUG
}

std::size_t
frame::get_width() const
{
    return width_;
}

std::size_t
frame::get_height() const
{
    return height_;
}

std::size_t
frame::size() const
{
    return data_.size();
}

GLfloat *
frame::data()
{
    return data_.data();
}


GLfloat const *
frame::data() const
{
    return data_.data();
}

frame &
frame::median_filter(int radius)
{
    // TODO: naive implementation for now

    if (radius <= 0) {
        return *this;
    }

    std::array<GLfloat, 9> sorted;

    auto const median = [&] (int i0, int j0) {
        int k = 0;
        for (int i = -radius; i <= radius; ++i) {
            for (int j = -radius; j <= radius; ++j) {
                sorted[k++] =
                    this->operator()((i0 + i) % height_, (j0 + j) % width_);
            }
        }
        std::sort(sorted.begin(), sorted.end());
        return sorted[4];
    };

    for (auto i {0ul}; i < height_; ++i) {
        for (auto j {0ul}; j < width_; ++j) {
            this->operator()(i, j) = median(i, j);
        }
    }

    return *this;
}

GLfloat const &
frame::operator()(int i, int j) const
{
    return data_[width_*i + j];
}

GLfloat &
frame::operator()(int i, int j)
{
    return data_[width_*i + j];
}

frame &
frame::operator+=(frame const & f)
{
#ifdef VIEWER_DEBUG
    assert(get_width() == f.get_width());
    assert(get_height() == f.get_height());
#endif // VIEWER_DEBUG
    auto const size = this->size();
    for (auto i {0ul}; i < size; ++i) {
        data_[i] += f.data_[i];
    }
    return *this;
}

frame
frame::operator-(frame f) const
{
#ifdef VIEWER_DEBUG
    assert(get_width() == f.get_width());
    assert(get_height() == f.get_height());
#endif // VIEWER_DEBUG
    auto const size = this->size();
    for (auto i {0ul}; i < size; ++i) {
        f.data_[i] = (data_[i] - f.data_[i]);
    }
    return f;
}

// Local Variables:
// indent-tabs-mode: nil
// End:
