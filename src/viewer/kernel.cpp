#include "kernel.hpp"

kernel::apply(image const * const inputs, image * const outputs) {
    apply_impl(inputs, outputs);
}

// Local Variables:
// indent-tabs-mode: nil
// End:
