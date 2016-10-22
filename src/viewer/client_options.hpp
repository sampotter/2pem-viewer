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
private:
    client_options(std::string const & hostname, std::string const & port,
                   std::size_t img_width, std::size_t img_height,
                   std::size_t slm_width, std::size_t slm_height,
                   std::size_t gs_iter_count);
    std::string hostname_;
    std::string port_;
    std::size_t img_width_;
    std::size_t img_height_;
    std::size_t slm_width_;
    std::size_t slm_height_;
    std::size_t gs_iter_count_;
};

#endif // __CLIENT_OPTIONS_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
