#include "client_slm_state.hpp"

#include <functional>
#include <iostream>

client_slm_state::client_slm_state(client_signal_dispatcher & signal_dispatcher)
{
	std::function<void(double, double)> left_mouse_down = [] (double xpos, double ypos) {
		std::cout << xpos << ", " << ypos << std::endl;
	};

	signal_dispatcher.connect_left_mouse_down_slot(left_mouse_down);
}
	
