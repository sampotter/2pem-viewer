#ifndef __KERNEL_HPP__
#define __KERNEL_HPP__

#include "image.hpp"

struct kernel {
    void apply(image const * const inputs, image * const outputs) const;
private:
    virtual void apply_impl(image const * const inputs,
                            image * const outputs) const = 0;
};

#endif // __KERNEL_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
