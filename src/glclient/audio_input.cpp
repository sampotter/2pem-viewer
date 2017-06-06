#include "client_audio_input.hpp"

#include <stdexcept>
#include <string>

#include "portaudio.h"

static void handle_error(PaError error)
{
    if (error != paNoError) {
        throw std::runtime_error(
            "PortAudio error: " + std::string {Pa_GetErrorText(error)});
    }
}

client_audio_input::client_audio_input()
{
    handle_error(Pa_Initialize());
}

client_audio_input::~client_audio_input()
{
    handle_error(Pa_Terminate());
}

// Local Variables:
// indent-tabs-mode: nil
// End:
