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
}

glfw::window::~window()
{
	glfwDestroyWindow(window_);
}

void
glfw::window::makeContextCurrent() const
{
	glfwMakeContextCurrent(window_);
}

bool
glfw::window::shouldClose() const
{
	return glfwWindowShouldClose(window_);
}

rect
glfw::window::getFramebufferSize() const
{
	rect size;
	glfwGetFramebufferSize(window_, &size.width, &size.height);
	return size;
}

void
glfw::window::swapBuffers() const
{
	glfwSwapBuffers(window_);
}

void
glfw::window::setCloseCallback(GLFWwindowclosefun cbfun) const
{
	glfwSetWindowCloseCallback(window_, cbfun);
}
