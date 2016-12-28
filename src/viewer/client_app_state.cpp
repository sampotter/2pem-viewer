#include "client_app_state.hpp"

#include <algorithm>

client_app_state::client_app_state(client_options const & options):
    options_ {options},
    asio_state_ {options},
#ifndef _WIN64
    osc_state_ {options_.get_osc_port()},
#endif // _WIN64
    input_window_ {
        options.get_img_width(),
        options.get_img_height(),
        signal_dispatcher_,
    },
    slm_window_ {
        options.get_slm_width(),
        options.get_slm_height()
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
        asio_state_.receive_frame(frame_);
        if (template_frame_) {
            frame_.align(*template_frame_);
        }
        
        input_window_.redraw(frame_, slm_state_.get_target_points());
        
        glfw::pollEvents();

        if (slm_state_.phase_mask_recomputed()) {
            slm_window_.redraw(slm_state_.get_recomputed_phase_mask());
        }
    } while (!input_window_.should_close());
}

// Local Variables:
// indent-tabs-mode: nil
// End:
