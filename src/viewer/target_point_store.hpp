#ifndef __TARGET_POINT_STORE_HPP__
#define __TARGET_POINT_STORE_HPP__

#include <vector>

#include "target_point.hpp"

struct target_point_store {
    void add_target_point(double x, double y);
    void remove_target_point(double x, double y);
    std::vector<target_point> const & get_target_points() const;
private:
    std::vector<target_point> pts_;
};

#endif // __TARGET_POINT_STORE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
