#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include "image_parameters.hpp"

struct image {
    image(image_parameters & parameters);
private:
    void * _data;
    image_parameters _parameters;
};

#endif // __IMAGE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
