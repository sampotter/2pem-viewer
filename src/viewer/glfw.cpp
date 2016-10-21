#include "glfw.hpp"

#include <cassert>
#include <limits>
#include <stdexcept>

rect::rect() {}

rect::rect(int width, int height): width(width), height(height) {}

void
glfw::init()
{
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}
}

void
glfw::terminate()
{
	glfwTerminate();
}

void
glfw::pollEvents()
{
	glfwPollEvents();
}

glfw::library::library()
{
	glfw::init();
}

glfw::library::~library()
{
	glfw::terminate();
}

glfw::window::window(std::size_t width, std::size_t height, const char * title,
					 GLFWmonitor * monitor, GLFWwindow * share):
	width_ {static_cast<int>(width)},
	height_ {static_cast<int>(height)},
	title_ {title ? title : "Untitled"},
	monitor_ {monitor},
	share_ {share},
	window_ {glfwCreateWindow(width_, height_, title_, monitor_, share_)}
{
#ifdef NUFFT_DEBUG
	assert(width <= std::numeric_limits<int>::max());
	assert(height <= std::numeric_limits<int>::max());
#endif // NUFFT_DEBUG
	if (window_ == nullptr) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFWwindow");
	}
	glfwSetWindowUserPointer(window_, (void *) this);
	glfwSetCursorPosCallback(window_, glfw::window::cursor_pos_callback);
	glfwSetMouseButtonCallback(window_, glfw::window::mouse_button_callback);
	glfwSetScrollCallback(window_, glfw::window::scroll_callback);
}

glfw::window::~window()
{
	glfwDestroyWindow(window_);
}

void
glfw::window::make_context_current() const
{
	glfwMakeContextCurrent(window_);
}

bool
glfw::window::should_close() const
{
	return glfwWindowShouldClose(window_);
}

rect
glfw::window::get_framebuffer_size() const
{
	rect size;
	glfwGetFramebufferSize(window_, &size.width, &size.height);
	return size;
}

void
glfw::window::swap_buffers() const
{
	glfwSwapBuffers(window_);
}

void
glfw::window::hide() const
{
	glfwHideWindow(window_);
}

void
glfw::window::cursor_pos_callback(GLFWwindow * window, double xpos, double ypos)
{
	auto glfw_window = (glfw::window *) glfwGetWindowUserPointer(window);
	glfw_window->cursor_pos_callback_impl(xpos, ypos);
}

void
glfw::window::mouse_button_callback(GLFWwindow * window, int button, int action,
									int mods)
{
	auto glfw_window = (glfw::window *) glfwGetWindowUserPointer(window);
	glfw_window->mouse_button_callback_impl(button, action, mods);
}

void
glfw::window::scroll_callback(GLFWwindow * window, double xoffset,
							  double yoffset)
{
	auto glfw_window = (glfw::window *) glfwGetWindowUserPointer(window);
	glfw_window->scroll_callback_impl(xoffset, yoffset);
}
