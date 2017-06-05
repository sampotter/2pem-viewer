#ifndef __IMAGE_PARAMETERS_HPP__
#define __IMAGE_PARAMETERS_HPP__

#include <cstddef>

enum class image_format {
    UINT16
};

struct image_parameters {
private:
    size_t _width;
    size_t _height;
    size_t _depth;
    size_t _channels;
    image_format _format;
    // TODO: bool _sparse;
};

#endif // __IMAGE_PARAMETERS_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
