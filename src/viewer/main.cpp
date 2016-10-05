#include <cassert>
#include <cinttypes>
#include <complex>
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

// none...

/**
 * RAW file information.
 */

auto const RAW_PATH = "/Volumes/Molly/Data/Spont0001/Image_0001_0001.raw";
// auto const RAW_PATH = "/Volumes/Molly/Dropbox/Research/two-photon/src/m/regseq/blah";
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

void check_for_errors() {
#ifdef VIEWER_DEBUG
	assert(!glGetError());
#endif // VIEWER_DEBUG
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

	glDisable(GL_DITHER);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);

	// glEnable(GL_TEXTURE_RECTANGLE_ARB);

	// Create, bind, and specify texture.

	GLuint texture;
	glGenTextures(1, &texture);
	check_for_errors();

	glActiveTexture(GL_TEXTURE0 + 0);
	check_for_errors();
	glBindTexture(GL_TEXTURE_2D, texture);
	check_for_errors();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	check_for_errors();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	check_for_errors();

	// Set up PBOs.

	GLuint pixel_buffer;
	glGenBuffers(1, &pixel_buffer);
	check_for_errors();

	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixel_buffer);
	check_for_errors();
	glBufferData(
		GL_PIXEL_PACK_BUFFER,			// target
		2 * RAW_DIM_X * RAW_DIM_Y,		// size
		static_cast<GLvoid *>(data),	// data
		GL_DYNAMIC_DRAW					// usage
		);
	check_for_errors();
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	check_for_errors();

	// TODO: only do this once -- move it to before the while loop
		
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixel_buffer);
	check_for_errors();
	glActiveTexture(GL_TEXTURE0 + 0);
	check_for_errors();
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
	check_for_errors();
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	check_for_errors();
	
    // Create vertex data and corresponding VBO.

	GLfloat verts[] = {
		1.0, 1.0,	// top right
		1.0, -1.0,	// lower right
		-1.0, -1.0, // lower left
		-1.0, 1.0,	// top left
	};

	GLuint verts_vbo;
	glGenBuffers(1, &verts_vbo);
	check_for_errors();
	glBindBuffer(GL_ARRAY_BUFFER, verts_vbo);
	check_for_errors();
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	check_for_errors();

	// Create texcoord data and corresponding VBO.

	GLfloat texcoords[] = {
		0.0, 0.0,	// lower left
		1.0, 0.0,	// lower right
		1.0, 1.0,	// upper right
		0.0, 1.0,	// upper left
	};

	GLuint texcoords_vbo;
	glGenBuffers(1, &texcoords_vbo);
	check_for_errors();
	glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
	check_for_errors();
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
	check_for_errors();

	// Load and compile vertex shader.

	auto const vertex_shader_string = string_from_file(VERTEX_SHADER_PATH);
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	check_for_errors();
	{
		char const * string = vertex_shader_string.data();
		glShaderSource(vertex_shader, 1, &string, nullptr);
		check_for_errors();
	}
	glCompileShader(vertex_shader);
	check_for_errors();
	{
		GLint compile_status = GL_FALSE;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
		check_for_errors();
		if (!compile_status) {
			std::cerr << "Failed to compile vertex shader" << std::endl;
			std::cerr << get_shader_info_log(vertex_shader) << std::endl;
		}
	}

	// Load and compile fragment shader.

	auto const fragment_shader_string = string_from_file(FRAGMENT_SHADER_PATH);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	check_for_errors();
	{
		char const * string = fragment_shader_string.data();
		GLint const length = fragment_shader_string.size();
		glShaderSource(fragment_shader, 1, &string, &length);
		check_for_errors();
	}
	glCompileShader(fragment_shader);
	check_for_errors();
	{
		GLint compile_status = GL_FALSE;
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
		check_for_errors();
		if (!compile_status) {
			std::cerr << "Failed to compile fragment shader" << std::endl;
			std::cerr << get_shader_info_log(fragment_shader) << std::endl;
		}
	}

	// Link shader program.

	GLuint shader_program = glCreateProgram();
	check_for_errors();
	glAttachShader(shader_program, vertex_shader);
	check_for_errors();
	glAttachShader(shader_program, fragment_shader);
	check_for_errors();
	glLinkProgram(shader_program);
	check_for_errors();
	{
		GLint link_status = GL_FALSE;
		glGetProgramiv(shader_program, GL_LINK_STATUS, &link_status);
		check_for_errors();
		if (!link_status) {
			std::cerr << "Failed to link shader program" << std::endl;
		}
	}

	// Get coord2d attribute location.

	GLint attrib_coord2d {glGetAttribLocation(shader_program, "coord2d")};
	check_for_errors();
	if (attrib_coord2d == -1) {
		std::cerr << "Failed to bind 'coord2d' attribute" << std::endl;
	}

	// Get texcoord attribute location.

	GLint attrib_texcoord {glGetAttribLocation(shader_program, "texcoord")};
	check_for_errors();
	if (attrib_texcoord == -1) {
		std::cerr << "Failed to bind 'texcoord' attribute" << std::endl;
	}

	// Get tex uniform location and set its value to GL_TEXTURE0.

	GLint uniform_tex {glGetUniformLocation(shader_program, "tex")};
	check_for_errors();
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
		check_for_errors();
		glBindTexture(GL_TEXTURE_2D, texture);
		check_for_errors();
		glUniform1i(uniform_tex, 0);
		check_for_errors();
	
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

		for (int i = 0; i < 16; ++i) {
			printf("%d ", data[i]);
		}
		printf("\n");

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
