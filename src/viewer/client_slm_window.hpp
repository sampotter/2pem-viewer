#ifndef __CLIENT_SLM_WINDOW_HPP__
#define __CLIENT_SLM_WINDOW_HPP__

#include <functional>

#include "client_options.hpp"
#include "client_signal_dispatcher.hpp"
#include "client_slm_window_gl_state.hpp"
#include "frame.hpp"
#include "glfw.hpp"

struct client_slm_window: public glfw::window {
	client_slm_window(std::size_t width, std::size_t height,
					  client_signal_dispatcher & signal_dispatcher);
	virtual ~client_slm_window();
private:
	virtual void cursor_pos_callback_impl(double xpos, double ypos);
	virtual void mouse_button_callback_impl(int button, int action, int mods);
	virtual void scroll_callback_impl(double xoffset, double yoffset);
	virtual void key_callback_impl(int key, int scancode, int action, int mods);

	void redraw(frame const & f) const;

	client_slm_window_gl_state gl_state_;
};

#endif // __CLIENT_SLM_WINDOW_HPP__
