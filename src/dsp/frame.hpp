#ifndef __FRAME_HPP__
#define __FRAME_HPP__

#include <vector>

#include <boost/asio.hpp>

struct frame {
    frame();
    frame(std::size_t width, std::size_t height);
    void translate(int ishift, int jshift);
    void align(frame const & other_frame);
    std::size_t get_width() const;
    std::size_t get_height() const;
    std::size_t size() const;
    float * data();
    float const * data() const;

    frame & median_filter(int radius);

    float const & operator()(int i, int j) const;
    float & operator()(int i, int j);
    frame & operator+=(frame const & f);
    frame operator-(frame f) const;

    template <class T> frame & operator+=(T const & t);
private:
    std::vector<float> data_;
    std::size_t width_;
    std::size_t height_;
};

#include "frame.impl.hpp"
#include "frame_ops.hpp"

#endif // __FRAME_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
