#include <cassert>
#include <cinttypes>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <GLFW/glfw3.h>

/**
 * OpenGL parameters.
 */

auto const NUM_PIXEL_BUFFERS = 2; // TODO: this might be a useless optimization

/**
 * RAW file information.
 */

auto const RAW_PATH = "/Volumes/Molly/Data/Spont0001/Image_0001_0001.raw";
std::size_t const RAW_TIME_PTS {15000};
std::size_t const RAW_DIM_X {512};
std::size_t const RAW_DIM_Y {512};
std::size_t const RAW_NUM_CHANS {4};
std::size_t const RAW_DISPLAY_CHAN {2};
std::size_t const RAW_ELT_BYTES {sizeof(uint16_t)};

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
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
	std::string info_log;
	info_log.resize(info_log_length);
	glGetShaderInfoLog(
		shader,
		info_log_length,
		nullptr,
		&info_log[0]
		);
	return info_log;
}

int main() {
	/**
	 * Open memory mapped file containing 2PEM data.
	 */

	boost::interprocess::file_mapping raw_file {
		RAW_PATH,
		boost::interprocess::read_only
	};
	
	boost::interprocess::mapped_region region {
		raw_file,
		boost::interprocess::read_only,
		0,
		RAW_DIM_X * RAW_DIM_Y * RAW_NUM_CHANS * RAW_ELT_BYTES
	};

	auto const frame_offset = (RAW_DISPLAY_CHAN - 1)*RAW_DIM_X*RAW_DIM_Y;
	auto data = static_cast<uint16_t *>(region.get_address()) + frame_offset;

	/**
	 * Setup GLFW3.
	 */	
	
	if (!glfwInit()) {
		std::cerr << "GLFW3 failed to initialize" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	GLFWwindow * window = glfwCreateWindow(
		RAW_DIM_X,
		RAW_DIM_Y,
		"test",
		nullptr,
		nullptr
		);
	if (window == nullptr) {
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	/**
	 * OpenGL initialization.
	 */

	// Create, bind, and specify texture.

	GLuint texture {0};
	glGenTextures(1, &texture);
	assert(!glGetError());
	glActiveTexture(GL_TEXTURE0 + 0);
	assert(!glGetError());
	glBindTexture(GL_TEXTURE_2D, texture);
	assert(!glGetError());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	assert(!glGetError());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	assert(!glGetError());

	GLuint pixel_buffers[NUM_PIXEL_BUFFERS];
	glGenBuffers(NUM_PIXEL_BUFFERS, pixel_buffers);
	assert(!glGetError());

	for (auto const & pixel_buffer: pixel_buffers) {
		glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer);
		assert(!glGetError());
		glBufferData(
			GL_PIXEL_PACK_BUFFER,			// target
			2 * RAW_DIM_X * RAW_DIM_Y,		// size
			static_cast<GLvoid *>(data),	// data
			GL_DYNAMIC_DRAW					// usage
			);
		assert(!glGetError());
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		assert(!glGetError());

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixel_buffer);
		assert(!glGetError());
		glActiveTexture(GL_TEXTURE0 + 0);
		assert(!glGetError());
		glTexImage2D(
			GL_TEXTURE_2D,					// target
			0,								// level
			GL_LUMINANCE16,					// internalformat
			RAW_DIM_X,						// width
			RAW_DIM_Y,						// height
			0,								// border
			GL_BGRA,						// format
			GL_UNSIGNED_SHORT_1_5_5_5_REV,	// type
			nullptr							// pixels
			);
		assert(!glGetError());
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		assert(!glGetError());
	}

	// Create vertex data and corresponding VBO.

	GLfloat verts[] = {
		1.0, 1.0,	// top right
		1.0, -1.0,	// lower right
		-1.0, -1.0, // lower left
		-1.0, 1.0,	// top left
	};

	GLuint verts_vbo;
	glGenBuffers(1, &verts_vbo);
	assert(!glGetError());
	glBindBuffer(GL_ARRAY_BUFFER, verts_vbo);
	assert(!glGetError());
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	assert(!glGetError());

	// Create texcoord data and corresponding VBO.

	GLfloat texcoords[] = {
		0.0, 0.0,	// lower left
		1.0, 0.0,	// lower right
		1.0, 1.0,	// upper right
		0.0, 1.0,	// upper left
	};

	GLuint texcoords_vbo;
	glGenBuffers(1, &texcoords_vbo);
	assert(!glGetError());
	glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
	assert(!glGetError());
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
	assert(!glGetError());

	// Load and compile vertex shader.

	auto const vertex_shader_string = string_from_file(VERTEX_SHADER_PATH);
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	assert(!glGetError());
	{
		char const * string = vertex_shader_string.data();
		glShaderSource(vertex_shader, 1, &string, nullptr);
		assert(!glGetError());
	}
	glCompileShader(vertex_shader);
	assert(!glGetError());
	{
		GLint compile_status = GL_FALSE;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
		assert(!glGetError());
		if (!compile_status) {
			std::cerr << "Failed to compile vertex shader" << std::endl;
			std::cerr << get_shader_info_log(vertex_shader) << std::endl;
		}
	}

	// Load and compile fragment shader.

	auto const fragment_shader_string = string_from_file(FRAGMENT_SHADER_PATH);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	assert(!glGetError());
	{
		char const * string = fragment_shader_string.data();
		GLint const length = fragment_shader_string.size();
		glShaderSource(fragment_shader, 1, &string, &length);
		assert(!glGetError());
	}
	glCompileShader(fragment_shader);
	assert(!glGetError());
	{
		GLint compile_status = GL_FALSE;
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
		assert(!glGetError());
		if (!compile_status) {
			std::cerr << "Failed to compile fragment shader" << std::endl;
			std::cerr << get_shader_info_log(fragment_shader) << std::endl;
		}
	}

	// Link shader program.

	GLuint shader_program = glCreateProgram();
	assert(!glGetError());
	glAttachShader(shader_program, vertex_shader);
	assert(!glGetError());
	glAttachShader(shader_program, fragment_shader);
	assert(!glGetError());
	glLinkProgram(shader_program);
	assert(!glGetError());
	{
		GLint link_status = GL_FALSE;
		glGetProgramiv(shader_program, GL_LINK_STATUS, &link_status);
		assert(!glGetError());
		if (!link_status) {
			std::cerr << "Failed to link shader program" << std::endl;
		}
	}

	// Get coord2d attribute location.

	GLint attrib_coord2d {glGetAttribLocation(shader_program, "coord2d")};
	assert(!glGetError());
	if (attrib_coord2d == -1) {
		std::cerr << "Failed to bind 'coord2d' attribute" << std::endl;
	}

	// Get texcoord attribute location.

	GLint attrib_texcoord {glGetAttribLocation(shader_program, "texcoord")};
	assert(!glGetError());
	if (attrib_texcoord == -1) {
		std::cerr << "Failed to bind 'texcoord' attribute" << std::endl;
	}

	// Get tex uniform location and set its value to GL_TEXTURE0.

	GLint uniform_tex {glGetUniformLocation(shader_program, "tex")};
	assert(!glGetError());
	if (uniform_tex == -1) {
		std::cerr << "Failed to bind 'tex' uniform" << std::endl;
	}

	/**
	 * Main loop.
	 */

	glViewport(0, 0, RAW_DIM_X, RAW_DIM_Y);

	double prevTime = glfwGetTime();

	std::size_t frame = 0;
	while (!glfwWindowShouldClose(window) && frame < RAW_TIME_PTS) {
		std::cout << frame << ": ";

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shader_program);

		glActiveTexture(GL_TEXTURE0 + 0);
		assert(!glGetError());
		glBindTexture(GL_TEXTURE_2D, texture);
		assert(!glGetError());
		glUniform1i(uniform_tex, 0);
		assert(!glGetError());
	
		glEnableVertexAttribArray(attrib_coord2d);
		glBindBuffer(GL_ARRAY_BUFFER, verts_vbo);
		glVertexAttribPointer(
			attrib_coord2d, // index
			2,				// size
			GL_FLOAT,		// type
			GL_FALSE,		// normalized
			0,				// stride
			0				// pointer
			);

		glEnableVertexAttribArray(attrib_texcoord);
		glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
		glVertexAttribPointer(
			attrib_texcoord,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			0
			);
		
		glDrawArrays(GL_QUADS, 0, 4);
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Get the next frame from the memory mapped file and map it
		// to our OpenGL texture.

		++frame;
		auto const region_offset =
			frame * RAW_DIM_X * RAW_DIM_Y * RAW_NUM_CHANS * RAW_ELT_BYTES;

		region = boost::interprocess::mapped_region {
			raw_file,
			boost::interprocess::read_only,
			static_cast<boost::interprocess::offset_t>(region_offset),
			RAW_DIM_X * RAW_DIM_Y * RAW_NUM_CHANS * RAW_ELT_BYTES
		};
			
		auto data = static_cast<uint16_t *>(region.get_address()) + frame_offset;

		auto const pixel_buffer = pixel_buffers[frame % NUM_PIXEL_BUFFERS];

		glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer);
		glBufferSubData(
			GL_PIXEL_PACK_BUFFER,		// target
			0,							// offset
			2 * RAW_DIM_X * RAW_DIM_Y,	// size
			static_cast<GLvoid *>(data)	// data
			);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixel_buffer);
		glTexSubImage2D(
			GL_TEXTURE_2D,					// target
			0,								// level
			0,								// xoffset
			0,								// yoffset
			RAW_DIM_X,						// width
			RAW_DIM_Y,						// height
			GL_BGRA,						// format
			GL_UNSIGNED_SHORT_1_5_5_5_REV,	// type
			nullptr							// pixels
			);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

		std::cout << (glfwGetTime() - prevTime)/frame << std::endl;
	}

	/**
	 * Cleanup OpenGL.
	 */

	glDeleteTextures(1, &texture);

	/**
	 * Cleanup GLFW3.
	 */

	glfwTerminate();
}
