#ifndef __LENS_PARAMETERS_HPP__
#define __LENS_PARAMETERS_HPP__

struct lens_parameters {
	double focal_length; // in mm
	double wavelength; // in nm
	struct {
		std::size_t x;
		std::size_t y;
	} origin; // in pixel indices
			  // TODO: convert to mm/same unit as slm
};

#endif // __LENS_PARAMETERS_HPP__
