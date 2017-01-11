#ifndef __FRAME_OPS_HPP__
#define __FRAME_OPS_HPP__

#include "frame.hpp"

template <class T>
frame
operator/(frame const & f, T const & t);

#include "frame_ops.impl.hpp"

#endif // __FRAME_OPS_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
