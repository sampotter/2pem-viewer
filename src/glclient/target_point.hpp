#ifndef __TARGET_POINT_HPP__
#define __TARGET_POINT_HPP__

#include <iostream>

struct target_point {
    target_point(double x, double y);
    double get_x() const;
    double get_y() const;
    bool contains(double x, double y) const;
    
    static double screen_axicon_radius;
private:
    double x_;
    double y_;
};

std::ostream & operator<<(std::ostream & ostream, target_point const & pt);

#endif // __TARGET_POINT_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
