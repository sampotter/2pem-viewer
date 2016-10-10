#include "target_point_store.hpp"

void
target_point_store::add_target_point(double x, double y)
{
	std::cout << "add_target_point: " << x << ", " << y << std::endl;
	pts_.emplace_back(x, y);
	
	for (auto const & pt: pts_) {
		std::cout << pt << std::endl;
	}
}

void
target_point_store::remove_target_point(double x, double y)
{
	for (auto it = pts_.begin(); it != pts_.end(); ++it) {
		auto const pt = *it;
		if (pt.contains(x, y)) {
			pts_.erase(it);
			std::cout << "remove_target_point: " << x << ", " << y << std::endl;
			for (auto const & pt: pts_) {
				std::cout << pt << std::endl;
			}
			return;
		}
	}
}
