#include <glclient/app_state.hpp>

#include "config.hpp"

#include <algorithm>

#include <boost/optional.hpp>

#include <dsp/frame.hpp>
#include <dsp/statistics.hpp>
#include <glclient/asio_state.hpp>
#if USE_AUDIO_INPUT
#    include <glclient/audio_input.hpp>
#endif // USE_AUDIO_INPUT
#include <glclient/input_window.hpp>
#if USE_OSC
#    include <glclient/osc_state.hpp>
#endif // USE_OSC
#include <glclient/signal_dispatcher.hpp>
#include <glclient/slm_state.hpp>
#include <glclient/slm_window.hpp>

struct app_state::impl {
    impl(options const & options);
    void run();

    options options_;
    signal_dispatcher signal_dispatcher_;
    asio_state asio_state_;
#if USE_AUDIO_INPUT
    audio_input audio_input_;
#endif // USE_AUDIO_INPUT
#if USE_OSC
    osc_state osc_state_;
#endif // USE_OSC
    glfw::library library_;
    input_window input_window_;
    slm_window slm_window_;
    slm_state slm_state_;
    frame frame_;
    boost::optional<frame> template_frame_;
};

app_state::impl::impl(options const & options):
    options_ {options},
    asio_state_ {asio_state::from_options(options)},
#if USE_OSC
    osc_state_ {options_.get_osc_port()},
#endif // USE_OSC
    input_window_ {
        options.get_img_width(),
        options.get_img_height(),
        signal_dispatcher_,
    },
    slm_window_ {
        options.get_slm_parameters().resolution.width,
        options.get_slm_parameters().resolution.height
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

void
app_state::impl::run()
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

        while (slm_state_.peek_event() != slm_state::event::none) {
            switch (slm_state_.pop_event()) {
            case slm_state::event::became_hidden:
                slm_window_.clear();
                break;
            case slm_state::event::became_visible:
                slm_window_.redraw();
                break;
            case slm_state::event::phase_mask_recomputed:
                if (slm_state_.visible()) {
                    slm_window_.set_phase_mask(
                        slm_state_.get_recomputed_phase_mask());
                    slm_window_.redraw();
                }
                break;
            default: // will never happen
                assert(false);
                break;
            }
        }
    } while (!input_window_.should_close());
}

app_state
app_state::from_options(options const & options)
{
    return {options};
}

app_state
app_state::from_cli_args(int argc, char ** argv)
{
    return app_state::from_options(options::from_cli_args(argc, argv));
}

void
app_state::run()
{
    _m->run();
}

app_state::app_state(options const & options): _m {new impl(options)} {}

app_state::~app_state()
{
    delete _m;
}

// Local Variables:
// indent-tabs-mode: nil
// End:
