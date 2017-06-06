#ifndef __FRAME_IMPL_HPP__
#define __FRAME_IMPL_HPP__

template <class T>
frame &
frame::operator+=(T const & t)
{
    for (auto & x: data_) {
        x += t;
    }
    return *this;
}

#endif // __FRAME_IMPL_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
