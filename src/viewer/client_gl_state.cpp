#include "client_gl_state.hpp"

#include <string>

std::string
get_shader_info_log(GLuint shader)
{
    GLint info_log_length {0};
    gl::getShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
    std::string info_log(info_log_length, ' ');
    gl::getShaderInfoLog(shader, info_log_length, nullptr, &info_log[0]);
    return info_log;
}

void
client_gl_state::init() const
{
    gl::disable(GL_DITHER);
    gl::disable(GL_ALPHA_TEST);
    gl::disable(GL_BLEND);
    gl::disable(GL_STENCIL_TEST);
    gl::disable(GL_FOG);
    gl::disable(GL_DEPTH_TEST);
    gl::clearColor(0, 0, 0, 1);
}

void
client_gl_state::init_texture()
{
    gl::genTextures(1, &texture_);
    gl::activeTexture(GL_TEXTURE0 + 0);
    gl::bindTexture(GL_TEXTURE_2D, texture_);
    gl::texParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl::texParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void
client_gl_state::init_pbo(client_options const & options)
{
	{
		auto const w = options.get_img_width();
		auto const h = options.get_img_height();
		pbo_size_ = sizeof(GLfloat)*w*h;
	}

    gl::genBuffers(1, &pbo_);

    // Set up pixel pack buffer:

    gl::bindBuffer(GL_PIXEL_PACK_BUFFER, pbo_);
    gl::bufferData(
        GL_PIXEL_PACK_BUFFER,   // target
        pbo_size_,               // size
        nullptr,                // data
        GL_DYNAMIC_DRAW         // usage
        );
    gl::bindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    // Set up pixel unpack buffer:

    gl::bindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_);
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

void
client_gl_state::init_vertex_vbo()
{
    GLfloat verts[] = {
        1.0,  1.0,  // top right
        1.0, -1.0,  // lower right
        -1.0, -1.0, // lower left
        -1.0,  1.0, // top left
    };
    gl::genBuffers(1, &verts_vbo_);
    gl::bindBuffer(GL_ARRAY_BUFFER, verts_vbo_);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
}

void
client_gl_state::init_texcoords_vbo()
{
    GLfloat texcoords[] = {
        0.0, 0.0,   // lower left
        1.0, 0.0,   // lower right
        1.0, 1.0,   // upper right
        0.0, 1.0,   // upper left
    };
    gl::genBuffers(1, &texcoords_vbo_);
    gl::bindBuffer(GL_ARRAY_BUFFER, texcoords_vbo_);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords,
                   GL_STATIC_DRAW);
}

void
client_gl_state::init_shader(char const * source, GLenum shader_type,
							 GLuint * shader)
{
    *shader = gl::createShader(shader_type);
    gl::shaderSource(*shader, 1, &source, nullptr);
    gl::compileShader(*shader);

    GLint compile_status = GL_FALSE;
    gl::getShaderiv(*shader, GL_COMPILE_STATUS, &compile_status);
    if (!compile_status) {
        fprintf(stderr, "Failed to compile shader\n");
        fprintf(stderr, "%s\n", get_shader_info_log(*shader).c_str());
    }
}

void
client_gl_state::init_vertex_shader()
{
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
    init_shader(source, GL_VERTEX_SHADER, &vertex_shader_);
}

void
client_gl_state::init_fragment_shader()
{
    auto source =
        "#version 120\n"
        "\n"
        "varying vec2 v_texcoord;\n"
        "uniform sampler2D tex;\n"
        "\n"
        "void main(void) {\n"
        "    gl_FragColor = 2 * texture2D(tex, v_texcoord);\n"
        "}";
    init_shader(source, GL_FRAGMENT_SHADER, &fragment_shader_);
}

void
client_gl_state::init_locations()
{
    a_coord2d_ = gl::getAttribLocation(program_, "coord2d");
#ifdef VIEWER_DEBUG
    if (a_coord2d_ == -1) {
        fprintf(stderr, "Failed to bind 'coord2d' attribute\n");
    }
#endif // VIEWER_DEBUG

    a_texcoord_ = gl::getAttribLocation(program_, "texcoord");
#ifdef VIEWER_DEBUG
    if (a_texcoord_ == -1) {
        fprintf(stderr, "Failed to bind 'texcoord' attribute\n");
    }
#endif // VIEWER_DEBUG

    u_tex_ = gl::getUniformLocation(program_, "tex");
#ifdef VIEWER_DEBUG
    if (u_tex_ == -1) {
        fprintf(stderr, "Failed to bind 'tex' uniform\n");
    }
#endif // VIEWER_DEBUG
}

void
client_gl_state::init_shader_program()
{
    program_ = gl::createProgram();
    gl::attachShader(program_, vertex_shader_);
    gl::attachShader(program_, fragment_shader_);
    gl::linkProgram(program_);
    {
        GLint link_status = GL_FALSE;
        gl::getProgramiv(program_, GL_LINK_STATUS, &link_status);
        if (!link_status) {
            fprintf(stderr, "Failed to link shader program\n");
        }
    }
}

void
client_gl_state::update_viewport(glfw::window const & window) const
{
    auto const size = window.getFramebufferSize();
    gl::viewport(0, 0, size.width, size.height);
}

void
client_gl_state::buffer_frame(frame const & f) const
{
    gl::bindBuffer(GL_PIXEL_PACK_BUFFER, pbo_);
    gl::bufferSubData(
        GL_PIXEL_PACK_BUFFER,									// target
        0,														// offset
        pbo_size_,												// size
        static_cast<GLvoid *>(const_cast<GLfloat *>(f.data()))	// data
        );
    gl::bindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void
client_gl_state::texture_frame(frame const & f) const
{
    gl::bindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_);
    gl::texSubImage2D(
        GL_TEXTURE_2D,  // target
        0,              // level
        0,              // xoffset
        0,              // yoffset
        f.get_width(),	// width
        f.get_height(),	// height
        GL_LUMINANCE,   // format
        GL_FLOAT,       // type
        nullptr         // pixels
        );
    gl::bindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void
client_gl_state::draw_texture() const
{
    gl::clear(GL_COLOR_BUFFER_BIT);

    // TODO: From here...

    gl::activeTexture(GL_TEXTURE0 + 0);
    gl::bindTexture(GL_TEXTURE_2D, texture_);

    gl::useProgram(program_);

    gl::uniform(u_tex_, 0);

    gl::enableVertexAttribArray(a_coord2d_);
    gl::bindBuffer(GL_ARRAY_BUFFER, verts_vbo_);
    gl::vertexAttribPointer(a_coord2d_, 2, GL_FLOAT, GL_FALSE, 0, 0);

    gl::enableVertexAttribArray(a_texcoord_);
    gl::bindBuffer(GL_ARRAY_BUFFER, texcoords_vbo_);
    gl::vertexAttribPointer(a_texcoord_, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // ... to here: can all of this be done outside of the while loop?

    gl::drawArrays(GL_QUADS, 0, 4);
}

void
client_gl_state::cleanup()
{
    gl::deleteBuffers(1, &pbo_);
    gl::deleteBuffers(1, &verts_vbo_);
    gl::deleteBuffers(1, &texcoords_vbo_);
    gl::deleteProgram(program_);
    gl::deleteShader(fragment_shader_);
    gl::deleteShader(vertex_shader_);
    gl::deleteTextures(1, &texture_);
}
