#include "target_point_store.hpp"

void
target_point_store::add_target_point(double x, double y)
{
	for (auto const & pt: pts_) {
		if (pt.contains(x, y)) {
			return;
		}
	}
#ifdef NUFFT_DEBUG
	std::cout << "add_target_point: " << x << ", " << y << std::endl;
#endif // NUFFT_DEBUG
	pts_.emplace_back(x, y);
#ifdef NUFFT_DEBUG
	for (auto const & pt: pts_) {
		std::cout << pt << std::endl;
	}
#endif // NUFFT_DEBUG
}

void
target_point_store::remove_target_point(double x, double y)
{
	for (auto it = pts_.begin(); it != pts_.end(); ++it) {
		if (it->contains(x, y)) {
			pts_.erase(it);
#ifdef NUFFT_DEBUG
			std::cout << "remove_target_point: " << x << ", " << y << std::endl;
			for (auto const & pt: pts_) {
				std::cout << pt << std::endl;
			}
#endif // NUFFT_DEBUG
			return;
		}
	}
}

std::vector<target_point> const &
target_point_store::get_target_points() const
{
	return pts_;
}
