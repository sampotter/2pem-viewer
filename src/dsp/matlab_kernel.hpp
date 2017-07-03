#ifndef __MATLAB_KERNEL_HPP__
#define __MATLAB_KERNEL_HPP__

#include "kernel.hpp"

struct matlab_kernel: public kernel {
    matlab_kernel();
    ~matlab_kernel();
private:
    virtual void apply_impl(image const * const ins, image * const outs) const;
    struct impl;
    impl * _m;
};

#endif // __MATLAB_KERNEL_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
