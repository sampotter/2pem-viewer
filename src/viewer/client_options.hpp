#ifndef __CLIENT_OPTIONS_HPP__
#define __CLIENT_OPTIONS_HPP__

#include <string>

#include "lens_parameters.hpp"
#include "slm_parameters.hpp"

struct client_options {
    static client_options from_cli_args(int argc, char ** argv);
    std::string get_hostname() const;
    std::string get_port() const;
    std::size_t get_img_width() const;
    std::size_t get_img_height() const;
    std::size_t get_gs_iter_count() const;
#if USE_OSC
    std::size_t get_osc_port() const;
#endif // USE_OSC
    std::size_t get_num_mean_frames() const;
    slm_parameters const & get_slm_parameters() const;
    lens_parameters const & get_lens_parameters() const;
private:
#if USE_OSC
    client_options(std::string const & hostname, std::string const & port,
                   std::size_t img_width, std::size_t img_height,
                   std::size_t gs_iter_count, std::size_t osc_port,
                   std::size_t num_mean_frames, slm_parameters slm_params,
                   lens_parameters lens_params);
#else // USE_OSC
    client_options(std::string const & hostname, std::string const & port,
                   std::size_t img_width, std::size_t img_height,
                   std::size_t gs_iter_count, std::size_t num_mean_frames,
                   slm_parameters slm_params, lens_parameters lens_params);
#endif // USE_OSC
    std::string hostname_;
    std::string port_;
    std::size_t img_width_;
    std::size_t img_height_;
    std::size_t gs_iter_count_;
#if USE_OSC
    std::size_t osc_port_;
#endif // USE_OSC
    std::size_t num_mean_frames_;
    slm_parameters slm_params_;
    lens_parameters lens_params_;
};

#endif // __CLIENT_OPTIONS_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
