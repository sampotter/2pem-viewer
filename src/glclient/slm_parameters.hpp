#ifndef __SLM_PARAMETERS_HPP__
#define __SLM_PARAMETERS_HPP__

struct slm_parameters {
	struct {
		std::size_t width;
		std::size_t height;
	} resolution;
	struct {
		double width;
		double height;
	} dimensions;
};

#endif // __SLM_PARAMETERS_HPP__
