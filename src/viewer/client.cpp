#include <cstdio>
#include <string>

#include <boost/asio.hpp>
#include <boost/optional.hpp>

#include "client_options.hpp"
#include "frame.hpp"
#include "glfw.hpp"
#include "gl.hpp"

static_assert(sizeof(GLfloat) == sizeof(float)); // guaranteed?

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
GLsizeiptr pbo_size;

void initPBO(client_options const & options) {
    pbo_size = sizeof(GLfloat)*options.get_img_width()*options.get_img_height();

    gl::genBuffers(1, &pbo);

    // Set up pixel pack buffer:

    gl::bindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
    gl::bufferData(
        GL_PIXEL_PACK_BUFFER,   // target
        pbo_size,               // size
        nullptr,                // data
        GL_DYNAMIC_DRAW         // usage
        );
    gl::bindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    // Set up pixel unpack buffer:

    gl::bindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    gl::activeTexture(GL_TEXTURE0 + 0);
    gl::texImage2D(
        GL_TEXTURE_2D,				// target
        0,							// level
        GL_LUMINANCE,				// internalformat
        options.get_img_width(),    // width
        options.get_img_height(),   // height
        0,							// border
        GL_LUMINANCE,				// format
        GL_FLOAT,					// type
        nullptr						// pixels
        );
    gl::bindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

GLuint verts_vbo;

void initVertexVBO() {
    GLfloat verts[] = {
        1.0,  1.0,  // top right
        1.0, -1.0,  // lower right
        -1.0, -1.0, // lower left
        -1.0,  1.0, // top left
    };
    gl::genBuffers(1, &verts_vbo);
    gl::bindBuffer(GL_ARRAY_BUFFER, verts_vbo);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
}

GLuint texcoords_vbo;

void initTexcoordsVBO() {
    GLfloat texcoords[] = {
        0.0, 0.0,   // lower left
        1.0, 0.0,   // lower right
        1.0, 1.0,   // upper right
        0.0, 1.0,   // upper left
    };
    gl::genBuffers(1, &texcoords_vbo);
    gl::bindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords,
                   GL_STATIC_DRAW);
}

void initShader(char const * source, GLenum shaderType, GLuint * shader) {
    *shader = gl::createShader(shaderType);
    gl::shaderSource(*shader, 1, &source, nullptr);
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
    auto source =
        "#version 120\n"
        "\n"
        "attribute vec2 coord2d;\n"
        "attribute vec2 texcoord;\n"
        "varying vec2 v_texcoord;\n"
        "\n"
        "void main(void) {\n"
        "    gl_Position = vec4(coord2d, 0.0, 1.0);\n"
        "    v_texcoord = texcoord;\n"
        "}";
    initShader(source, GL_VERTEX_SHADER, &vertex_shader);
}

GLuint fragment_shader;

void initFragmentShader() {
    auto source =
        "#version 120\n"
        "\n"
        "varying vec2 v_texcoord;\n"
        "uniform sampler2D tex;\n"
        "\n"
        "void main(void) {\n"
        "    gl_FragColor = 2 * texture2D(tex, v_texcoord);\n"
        "}";
    initShader(source, GL_FRAGMENT_SHADER, &fragment_shader);
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

void processFrame(
    glfw::window const & window,
    frame & main_frame,
    boost::optional<frame &> template_frame = boost::none)
{
    main_frame.receive(sock);
    if (template_frame) {
        main_frame.align(*template_frame);
    }
    updateViewport(window);
    main_frame.buffer_and_teximage(pbo, pbo_size);
    drawTexture();
    window.swapBuffers();
    glfw::pollEvents();
    gl::flush();
}

void disconnect() {
    boost::system::error_code error_code;
    sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error_code);
    sock.close();
    io.stop();
}

void cleanupOpenGL() {
    gl::deleteBuffers(1, &pbo);
    gl::deleteBuffers(1, &verts_vbo);
    gl::deleteBuffers(1, &texcoords_vbo);
    gl::deleteProgram(program);
    gl::deleteShader(fragment_shader);
    gl::deleteShader(vertex_shader);
    gl::deleteTextures(1, &texture);
}

void cleanupGLFW() {
    glfw::terminate();
}

void cleanup() {
    disconnect();
    cleanupOpenGL();
    cleanupGLFW();
}

void closeCallback(GLFWwindow *) {
    cleanup();
}

int main(int argc, char ** argv) {
	auto const options = client_options::from_cli_args(argc, argv);

    glfw::init();
    glfw::window window(options.get_img_width(), options.get_img_height());
    window.makeContextCurrent();
    window.setCloseCallback(closeCallback);

    initOpenGL();
    initTexture();
    initPBO(options);
    initVertexVBO();
    initTexcoordsVBO();
    initVertexShader();
    initFragmentShader();
    initShaderProgram();
    initLocations();

    connect(options.get_hostname(), options.get_port());

	frame main_frame {options.get_img_width(), options.get_img_height()};
	frame template_frame {options.get_img_width(), options.get_img_height()};

    processFrame(window, main_frame, template_frame);
    while (!window.shouldClose()) {
        processFrame(window, main_frame, template_frame);
    }

    cleanup();
}

// Local Variables:
// indent-tabs-mode: nil
// End:
