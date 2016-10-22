#include "client_app_state.hpp"

#include <algorithm>

client_app_state::client_app_state(client_options const & options):
    options_ {options},
    asio_state_ {options},
    input_window_ {
        options.get_img_width(),
        options.get_img_height(),
        signal_dispatcher_,
    },
    slm_window_ {
        options.get_slm_width(),
        options.get_slm_height(),
        signal_dispatcher_,
    },
    slm_state_ {
        options,
        signal_dispatcher_
    },
    frame_ {
        options_.get_img_width(),
        options_.get_img_height()
    },
    template_frame_ {boost::none}
{}

client_app_state
client_app_state::from_cli_args(int argc, char ** argv)
{
    return {client_options::from_cli_args(argc, argv)};
}

void
client_app_state::run()
{
    do {
        process_frame();
    } while (!input_window_.should_close());
}

void
client_app_state::process_frame()
{
    asio_state_.receive_frame(frame_);
    if (template_frame_) {
        frame_.align(*template_frame_);
    }
    input_window_.redraw(frame_, slm_state_.get_target_points());
    glfw::pollEvents();
    gl::flush();
}

// Local Variables:
// indent-tabs-mode: nil
// End:
