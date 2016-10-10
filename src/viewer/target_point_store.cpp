#include "target_point_store.hpp"

void
target_point_store::add_target_point(double xpos, double ypos)
{
	std::cout << "add_target_point: " << xpos << ", " << ypos << std::endl;
	pts_.emplace_back(xpos, ypos);
	
	for (auto const & pt: pts_) {
		std::cout << pt << std::endl;
	}
}
