#include <cassert>
#include <cinttypes>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/optional.hpp>

#include <fftw3.h>

#include "glfw.hpp"
#include "gl.hpp"

static_assert(sizeof(GLfloat) == sizeof(float)); // guaranteed?

struct options {
	std::string hostname;
	std::string port;
};

boost::optional<options>
parse_command_line_options(int argc, char ** argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
		return boost::none;
	}
	options opts;
	opts.hostname = argv[1];
	opts.port = argv[2];
	return opts;
}

/**
 * RAW file information.
 */

std::size_t const RAW_DIM_X {512};
std::size_t const RAW_DIM_Y {512};

/**
 * Shader file locations.
 */

auto const VERTEX_SHADER_PATH = "/Volumes/Molly/Dropbox/research/two-photon/viewer/src/viewer/vertex.glsl";
auto const FRAGMENT_SHADER_PATH = "/Volumes/Molly/Dropbox/research/two-photon/viewer/src/viewer/fragment.glsl";

std::string string_from_file(std::string path) {
	std::ifstream ifstream {path};
	std::stringstream stringstream;
	stringstream << ifstream.rdbuf();
	return stringstream.str();
}

std::string get_shader_info_log(GLuint shader) {
	GLint info_log_length {0};
	gl::getShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
	std::string info_log(info_log_length, ' ');
	gl::getShaderInfoLog(shader, info_log_length, nullptr, &info_log[0]);
	return info_log;
}


void initOpenGL() {
	gl::disable(GL_DITHER);
	gl::disable(GL_ALPHA_TEST);
	gl::disable(GL_BLEND);
	gl::disable(GL_STENCIL_TEST);
	gl::disable(GL_FOG);
	gl::disable(GL_DEPTH_TEST);
	
	gl::clearColor(0, 0, 0, 1);
}

GLuint texture;

void initTexture() {
	gl::genTextures(1, &texture);
	gl::activeTexture(GL_TEXTURE0 + 0);
	gl::bindTexture(GL_TEXTURE_2D, texture);
	gl::texParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gl::texParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

GLuint pbo;
GLsizeiptr const pbo_size = sizeof(GLfloat)*RAW_DIM_X*RAW_DIM_Y;

void initPBO() {
	gl::genBuffers(1, &pbo);

	// Set up pixel pack buffer:
	
	gl::bindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
	gl::bufferData(
		GL_PIXEL_PACK_BUFFER,	// target
		pbo_size,				// size
		nullptr,				// data
		GL_DYNAMIC_DRAW			// usage
		);
	gl::bindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	// Set up pixel unpack buffer:

	gl::bindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	gl::activeTexture(GL_TEXTURE0 + 0);
	gl::texImage2D(
		GL_TEXTURE_2D,	// target
		0,				// level
		GL_LUMINANCE,	// internalformat
		RAW_DIM_X,		// width
		RAW_DIM_Y,		// height
		0,				// border
		GL_LUMINANCE,	// format
		GL_FLOAT,		// type
		nullptr			// pixels
		);
	gl::bindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

GLuint verts_vbo;

void initVertexVBO() {
	GLfloat verts[] = {
		1.0,  1.0,	// top right
		1.0, -1.0,	// lower right
		-1.0, -1.0, // lower left
		-1.0,  1.0,	// top left
	};
	gl::genBuffers(1, &verts_vbo);
	gl::bindBuffer(GL_ARRAY_BUFFER, verts_vbo);
	gl::bufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
}

GLuint texcoords_vbo;

void initTexcoordsVBO() {
	GLfloat texcoords[] = {
		0.0, 0.0,	// lower left
		1.0, 0.0,	// lower right
		1.0, 1.0,	// upper right
		0.0, 1.0,	// upper left
	};
	gl::genBuffers(1, &texcoords_vbo);
	gl::bindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
	gl::bufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords,
				   GL_STATIC_DRAW);
}

void initShader(char const * path, GLenum shaderType, GLuint * shader) {
	auto const shader_string = string_from_file(path);
	*shader = gl::createShader(shaderType);
	{
		auto string = &shader_string[0];
		gl::shaderSource(*shader, 1, &string, nullptr);
	}
	gl::compileShader(*shader);

	GLint compile_status = GL_FALSE;
	gl::getShaderiv(*shader, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status) {
		fprintf(stderr, "Failed to compile shader\n");
		fprintf(stderr, "%s\n", get_shader_info_log(*shader).c_str());
	}
}

GLuint vertex_shader;

void initVertexShader() {
	initShader(VERTEX_SHADER_PATH, GL_VERTEX_SHADER, &vertex_shader);
}

GLuint fragment_shader;

void initFragmentShader() {
	initShader(FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER,
			   &fragment_shader);
}

GLuint program;

void initShaderProgram() {
	program = gl::createProgram();
	gl::attachShader(program, vertex_shader);
	gl::attachShader(program, fragment_shader);
	gl::linkProgram(program);
	{
		GLint link_status = GL_FALSE;
		gl::getProgramiv(program, GL_LINK_STATUS, &link_status);
		if (!link_status) {
			fprintf(stderr, "Failed to link shader program\n");
		}
	}
}

GLint a_coord2d;
GLint a_texcoord;
GLint u_tex;

void initLocations() {
	a_coord2d = gl::getAttribLocation(program, "coord2d");
	if (a_coord2d == -1) {
		fprintf(stderr, "Failed to bind 'coord2d' attribute\n");
	}

	a_texcoord = gl::getAttribLocation(program, "texcoord");
	if (a_texcoord == -1) {
		fprintf(stderr, "Failed to bind 'texcoord' attribute\n");
	}

	u_tex = gl::getUniformLocation(program, "tex");
	if (u_tex == -1) {
		fprintf(stderr, "Failed to bind 'tex' uniform\n");
	}
}

boost::asio::io_service io;
boost::asio::ip::tcp::socket sock(io);

void connect(std::string const & hostname, std::string const & port) {
	using namespace boost::asio;
	using namespace boost::asio::ip;

	tcp::resolver resolver(io);
	tcp::resolver::query query(hostname.c_str(), port.c_str());
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	connect(sock, endpoint_iterator);
}

void disconnect() {
	boost::system::error_code error_code;
	sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error_code);
	sock.close();
	io.stop();
}

void closeCallback(GLFWwindow *) {
	disconnect();
}

std::vector<float> frame(RAW_DIM_X*RAW_DIM_Y);

void receiveFrame() {
	boost::system::error_code error_code;
	auto data = (void *) &frame[0];
	auto size_in_bytes = sizeof(float)*frame.size();
	auto buffer = boost::asio::buffer(data, size_in_bytes);
	std::size_t len = boost::asio::read(sock, buffer);
#ifdef VIEWER_DEBUG
	fprintf(stderr, "Received frame (read %lu bytes, expected %lu)\n",
			len, sizeof(float)*frame.size());
#endif // VIEWER_DEBUG	
}

void updateViewport(glfw::window const & window) {
	auto const size = window.getFramebufferSize();
	gl::viewport(0, 0, size.width, size.height);
}

void drawTexture() {
	gl::clear(GL_COLOR_BUFFER_BIT);
		
	// TODO: From here...

	gl::activeTexture(GL_TEXTURE0 + 0);
	gl::bindTexture(GL_TEXTURE_2D, texture);
		
	gl::useProgram(program);
	
	gl::uniform(u_tex, 0);
	
	gl::enableVertexAttribArray(a_coord2d);
	gl::bindBuffer(GL_ARRAY_BUFFER, verts_vbo);
	gl::vertexAttribPointer(a_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);

	gl::enableVertexAttribArray(a_texcoord);
	gl::bindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
	gl::vertexAttribPointer(a_texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// ... to here: can all of this be done outside of the while loop?
		
	gl::drawArrays(GL_QUADS, 0, 4);
}

void loadFrame() {
	gl::bindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
	gl::bufferSubData(
		GL_PIXEL_PACK_BUFFER,				// target
		0,									// offset
		pbo_size,							// size
		static_cast<GLvoid *>(&frame[0])	// data
		);
	gl::bindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	gl::bindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	gl::texSubImage2D(
		GL_TEXTURE_2D,	// target
		0,				// level
		0,				// xoffset
		0,				// yoffset
		RAW_DIM_X,		// width
		RAW_DIM_Y,		// height
		GL_LUMINANCE,	// format
		GL_FLOAT,		// type
		nullptr			// pixels
		);
	gl::bindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void cleanupOpenGL() {
	gl::deleteTextures(1, &texture);
}

void cleanupGLFW() {
	glfw::terminate();
}

int main(int argc, char ** argv) {
	auto const opts = parse_command_line_options(argc, argv);
	if (!opts) std::exit(EXIT_FAILURE);

	glfw::init();
	glfw::window window(RAW_DIM_X, RAW_DIM_Y);
	window.makeContextCurrent();
	window.setCloseCallback(closeCallback);

	initOpenGL();
	initTexture();
	initPBO();
	initVertexVBO();
	initTexcoordsVBO();
	initVertexShader();
	initFragmentShader();
	initShaderProgram();
	initLocations();

	connect(opts->hostname, opts->port);

	while (!window.shouldClose()) {
		receiveFrame();
		updateViewport(window);
		loadFrame();
		drawTexture();
		window.swapBuffers();
		glfw::pollEvents();
		gl::flush();
	}

	cleanupOpenGL();
	cleanupGLFW();
}
