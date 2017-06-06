#ifndef __APP_STATE_HPP__
#define __APP_STATE_HPP__

#include "config.hpp"

#include <boost/optional.hpp>

#include "asio_state.hpp"
#if USE_AUDIO_INPUT
#    include "client_audio_input.hpp"
#endif // USE_AUDIO_INPUT
#include "input_window.hpp"
#include "options.hpp"
#if USE_OSC
#    include "client_osc_state.hpp"
#endif // USE_OSC
#include "signal_dispatcher.hpp"
#include "slm_state.hpp"
#include "slm_window.hpp"
#include "frame.hpp"

struct app_state {
    app_state(options const & options);
    static app_state from_cli_args(int argc, char ** argv);
    void run();
private:
    options options_;
    signal_dispatcher signal_dispatcher_;
    asio_state asio_state_;
#if USE_AUDIO_INPUT
    client_audio_input audio_input_;
#endif // USE_AUDIO_INPUT
#if USE_OSC
    client_osc_state osc_state_;
#endif // USE_OSC
    glfw::library library_;
    input_window input_window_;
    slm_window slm_window_;
    slm_state slm_state_;
    frame frame_;
    boost::optional<frame> template_frame_;
};

#endif // __APP_STATE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
