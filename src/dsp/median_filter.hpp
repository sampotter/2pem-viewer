#ifndef __MEDIAN_FILTER_HPP__
#define __MEDIAN_FILTER_HPP__

#include "convolution_kernel.hpp"

struct median_filter: public convolution_kernel {
private:
    virtual void apply_impl(image const * const inputs,
                            image * const outputs) const;
};

#endif // __MEDIAN_FILTER_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
