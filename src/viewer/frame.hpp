#ifndef __FRAME_HPP__
#define __FRAME_HPP__

#include <vector>

#include <boost/asio.hpp>

#include "gl.hpp"

struct frame {
	frame(std::size_t width, std::size_t height);
	void receive(boost::asio::ip::tcp::socket & socket);
	void buffer_and_teximage(GLuint pbo, GLsizeiptr pbo_size) const;
	void translate(int ishift, int jshift);
	void align(frame const & other_frame);
private:
	std::vector<GLfloat> data_;
	std::size_t width_;
	std::size_t height_;
};

#endif // __FRAME_HPP__
