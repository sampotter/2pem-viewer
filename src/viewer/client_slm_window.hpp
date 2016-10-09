#ifndef __CLIENT_SLM_WINDOW_HPP__
#define __CLIENT_SLM_WINDOW_HPP__

#include "glfw.hpp"

struct client_slm_window: public glfw::window {
	using glfw::window::window;
private:
	virtual void cursor_pos_callback_impl(double xpos, double ypos);
	virtual void mouse_button_callback_impl(int button, int action, int mods);
	virtual void scroll_callback_impl(double xoffset, double yoffset);
};

#endif // __CLIENT_SLM_WINDOW_HPP__
