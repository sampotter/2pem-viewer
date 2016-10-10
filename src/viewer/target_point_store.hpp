#ifndef __TARGET_POINT_STORE_HPP__
#define __TARGET_POINT_STORE_HPP__

#include <vector>

#include "target_point.hpp"

struct target_point_store {
	void add_target_point(double x, double y);
private:
	std::vector<target_point> pts_;
};

#endif // __TARGET_POINT_STORE_HPP__
