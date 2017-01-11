#include "client_app_state.hpp"

#include <algorithm>

#include "frame.hpp"
#include "statistics.hpp"

client_app_state::client_app_state(client_options const & options):
    options_ {options},
    asio_state_ {options},
#if USE_OSC
    osc_state_ {options_.get_osc_port()},
#endif // USE_OSC
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
    // Running mean for filtering input frames
    static online_mean<frame> frame_mean {
        static_cast<int>(options_.get_num_mean_frames())
    };

    do {
        /**
         * Get frame onto input window:
         */

        // Receive frame from network
        asio_state_.receive_frame(frame_);

        // Process received frame (alignment, filtering, ...)
        if (template_frame_) {
            frame_.align(*template_frame_);
        }
        frame_mean.add_sample(frame_);
        if (frame_mean.has_mean()) {
            frame_ = frame_mean.get_mean().median_filter(1);
        
            // Draw frame
            input_window_.redraw(frame_, slm_state_.get_target_points());
        }

        /**
         * Poll for input events
         */
        glfw::pollEvents();

        /**
         * Pump SLM's event queue and handle events:
         */

        while (slm_state_.peek_event()) {
            switch (slm_state_.pop_event()) {
            case client_slm_state::event::became_hidden:
                slm_window_.clear();
                break;
            case client_slm_state::event::became_visible:
                slm_window_.redraw();
                break;
            case client_slm_state::event::phase_mask_recomputed:
                if (slm_state_.visible()) {
                    slm_window_.set_phase_mask(
                        slm_state_.get_recomputed_phase_mask());
                    slm_window_.redraw();
                }
                break;
            }
        }
    } while (!input_window_.should_close());
}

// Local Variables:
// indent-tabs-mode: nil
// End:
