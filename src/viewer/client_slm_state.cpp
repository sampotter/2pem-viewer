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
}
