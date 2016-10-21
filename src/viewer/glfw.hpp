#ifndef __VIEWER_GLFW_HPP__
#define __VIEWER_GLFW_HPP__

#include <cstddef>

#include <GLFW/glfw3.h>

struct rect {
	rect();
	rect(int width, int height);
	int width {0};
	int height {0};
};

namespace glfw {
	void init();
	void terminate();
	void pollEvents();

	struct library {
		library();
		~library();
	};
	
	struct window {
		window(std::size_t width = 640,
			   std::size_t height = 480,
			   char const * title = nullptr,
			   GLFWmonitor * monitor = nullptr,
			   GLFWwindow * share = nullptr);
		~window();
		void make_context_current() const;
		bool should_close() const;
		rect get_framebuffer_size() const;
		void swap_buffers() const;
		void hide() const;
	private:
		static void cursor_pos_callback(GLFWwindow * window, double xpos,
										double ypos);
		virtual void cursor_pos_callback_impl(double xpos, double ypos) = 0;

		static void mouse_button_callback(GLFWwindow * window, int button,
										  int action, int mods);
		virtual void mouse_button_callback_impl(int button, int action,
												int mods) = 0;

		static void scroll_callback(GLFWwindow * window, double xoffset,
									double yoffset);
		virtual void scroll_callback_impl(double xoffset, double yoffset) = 0;

		static void key_callback(GLFWwindow * window, int key, int scancode,
								 int action, int mods);
		virtual void key_callback_impl(int key, int scancode, int action,
									   int mods) = 0;

		int width_;
		int height_;
		char const * title_;
		GLFWmonitor * monitor_;
		GLFWwindow * share_;
		GLFWwindow * window_;
	};
}

#endif // __VIEWER_GLFW_HPP__
