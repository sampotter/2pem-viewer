#ifndef __FRAME_OPS_IMPL_HPP__
#define __FRAME_OPS_IMPL_HPP__

// TODO: this probably doesn't work right now

template <class T>
frame
operator+(frame const & f, T const & t)
{
    frame g = f;
    auto data = g.data();
    auto const size = g.size();
    for (auto i {0ul}; i < size; ++i) {
        data[i] += t;
    }
    return g;
}

template <class T>
frame
operator/(frame const & f, T const & t)
{
    frame g = f;
    auto data = g.data();
    auto const size = g.size();
    for (auto i {0ul}; i < size; ++i) {
        data[i] /= t;
    }
    return g;
}

#endif // __FRAME_OPS_IMPL_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
