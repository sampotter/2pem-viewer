#include "kernel.hpp"

void
kernel::apply(image const * const inputs, image * const outputs) const {
    apply_impl(inputs, outputs);
}

// Local Variables:
// indent-tabs-mode: nil
// End:
