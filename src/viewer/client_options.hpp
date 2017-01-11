#ifndef __CLIENT_OPTIONS_HPP__
#define __CLIENT_OPTIONS_HPP__

#include <string>

struct client_options {
    static client_options from_cli_args(int argc, char ** argv);
    std::string get_hostname() const;
    std::string get_port() const;
    std::size_t get_img_width() const;
    std::size_t get_img_height() const;
    std::size_t get_slm_width() const;
    std::size_t get_slm_height() const;
    std::size_t get_gs_iter_count() const;
#if USE_OSC
    std::size_t get_osc_port() const;
#endif // USE_OSC
    std::size_t get_num_mean_frames() const;
private:
#if USE_OSC
    client_options(std::string const & hostname, std::string const & port,
                   std::size_t img_width, std::size_t img_height,
                   std::size_t slm_width, std::size_t slm_height,
                   std::size_t gs_iter_count, std::size_t osc_port,
                   std::size_t num_mean_frames);
#else // USE_OSC
    client_options(std::string const & hostname, std::string const & port,
                   std::size_t img_width, std::size_t img_height,
                   std::size_t slm_width, std::size_t slm_height,
                   std::size_t gs_iter_count, std::size_t num_mean_frames);
#endif // USE_OSC
    std::string hostname_;
    std::string port_;
    std::size_t img_width_;
    std::size_t img_height_;
    std::size_t slm_width_;
    std::size_t slm_height_;
    std::size_t gs_iter_count_;
#if USE_OSC
    std::size_t osc_port_;
#endif // USE_OSC
    std::size_t num_mean_frames_;
};

#endif // __CLIENT_OPTIONS_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
