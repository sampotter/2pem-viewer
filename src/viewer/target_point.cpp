#include "target_point.hpp"

double target_point::screen_axicon_radius = 25;

target_point::target_point(double x, double y): x_ {x}, y_ {y} {}

double
target_point::get_x() const
{
    return x_;
}

double
target_point::get_y() const
{
    return y_;
}

bool
target_point::contains(double x, double y) const
{
    double dx = x_ - x;
    double dy = y_ - y;
    double r = target_point::screen_axicon_radius;
    return dx*dx + dy*dy <= r*r;
}

std::ostream & operator<<(std::ostream & ostream, target_point const & pt)
{
    return ostream << "{" << pt.get_x() << ", " << pt.get_y() << "}";
}

// Local Variables:
// indent-tabs-mode: nil
// End:
