#include "client_slm_state.hpp"

#include <functional>
#include <iostream>

client_slm_state::client_slm_state(client_signal_dispatcher & signal_dispatcher)
{
	{
		auto func = std::function<void(double, double)> {
			[this] (double x, double y) {
				point_store_.add_target_point(x, y);
			}
		};
		signal_dispatcher.connect_left_mouse_down_slot(func);
	}
	{
		auto func = std::function<void(double, double)> {
			[this] (double x, double y) {
				point_store_.remove_target_point(x, y);
			}
		};
		signal_dispatcher.connect_right_mouse_down_slot(func);
	}
}

std::vector<target_point> const &
client_slm_state::get_target_points() const
{
	return point_store_.get_target_points();
}
