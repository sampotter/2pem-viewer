#include "client_app_state.hpp"

client_app_state
client_app_state::from_cli_args(int argc, char ** argv)
{
	return {client_options::from_cli_args(argc, argv)};
}

client_app_state::~client_app_state()
{
	gl_state_.cleanup();
	asio_state_.disconnect();
	glfw_state_.cleanup();
}

void
client_app_state::init()
{
	asio_state_.connect(options_);

	glfw_state_.init();

	gl_state_.init();
	gl_state_.init_texture();
	gl_state_.init_pbo(options_);
	gl_state_.init_vertex_vbo();
	gl_state_.init_texcoords_vbo();
	gl_state_.init_scope_frame_vshader();
	gl_state_.init_scope_frame_fshader();
	gl_state_.init_scope_frame_shader_program();
	gl_state_.init_locations();

	initialized_ = true;
}

void
client_app_state::process_frame(client_error & error)
{
	asio_state_.receive_frame(frame_, error);
	switch (error) {
	default:
		break;
	}

    if (template_frame_) {
        frame_.align(*template_frame_);
    }
	gl_state_.update_viewport(glfw_state_.get_input_window());
    gl_state_.buffer_frame(frame_);
	gl_state_.texture_frame(frame_);
    gl_state_.draw_texture();
    glfw_state_.get_input_window().swap_buffers();
    glfw::pollEvents();
    gl::flush();
}

void
client_app_state::run()
{
	if (!initialized_) {
		init();
	}
	client_error error {client_error::success};
	do {
		process_frame(error);
	} while (!glfw_state_.get_input_window().should_close());
}

client_app_state::client_app_state(client_options const & options):
	options_ {options},
	glfw_state_ {options, signal_dispatcher_},
	slm_state_ {signal_dispatcher_},
	frame_ {options_.get_img_width(), options_.get_img_height()},
	template_frame_ {boost::none}
{}
