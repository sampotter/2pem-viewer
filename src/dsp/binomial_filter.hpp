#ifndef __BINOMIAL_FILTER_HPP__
#define __BINOMIAL_FILTER_HPP__

#include "convolution_kernel.hpp"

/**
 * NAME: Binomial Filter
 */
struct binomial_filter: public convolution_kernel {
private:
    virtual void apply_impl(image const * const inputs,
                            image * const outputs) const;
};

#endif // __BINOMIAL_FILTER_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
