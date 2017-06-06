#ifndef __CONVOLUTION_KERNEL_HPP__
#define __CONVOLUTION_KERNEL_HPP__

#include "kernel.hpp"

enum class edge_behavior {
    REPEAT,
    MIRROR
};

struct convolution_kernel: public kernel {
protected:
    edge_behavior get_edge_behavior() const;
private:
    edge_behavior _edge_behavior;
};

#endif // __CONVOLUTION_KERNEL_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
