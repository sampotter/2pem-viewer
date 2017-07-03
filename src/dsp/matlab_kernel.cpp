#include <dsp/matlab_kernel.hpp>

#include <iostream>

#include "engine.h"

struct matlab_kernel::impl {
    impl();
    ~impl();
    Engine * _engine;
};

matlab_kernel::impl::impl(): _engine {engOpen(nullptr)} {}

matlab_kernel::impl::~impl()
{
    engClose(_engine);
}

matlab_kernel::matlab_kernel(): _m {new impl()}
{
    std::cout << "test" << std::endl;
}

matlab_kernel::~matlab_kernel()
{
    delete _m;
}

void matlab_kernel::apply_impl(image const * const ins, image * const outs) const
{
}

// Local Variables:
// indent-tabs-mode: nil
// End:
