#ifndef __FRAME_HPP__
#define __FRAME_HPP__

#include <vector>

#include <boost/asio.hpp>

#include "gl.hpp"

struct frame {
	frame(std::size_t width, std::size_t height);
	void translate(int ishift, int jshift);
	void align(frame const & other_frame);
	std::size_t get_width() const;
	std::size_t get_height() const;
	std::size_t size() const;
	GLfloat * data();
	GLfloat const * data() const;
private:
	std::vector<GLfloat> data_;
	std::size_t width_;
	std::size_t height_;
};

#endif // __FRAME_HPP__

