#ifndef __CLIENT_OPTIONS_HPP__
#define __CLIENT_OPTIONS_HPP__

#include <string>

struct client_options {
	static client_options from_cli_args(int argc, char ** argv);
	std::string get_hostname() const;
	std::string get_port() const;
	std::size_t get_img_width() const;
	std::size_t get_img_height() const;
private:
	client_options(std::string const & hostname, std::string const & port,
				   std::size_t img_width, std::size_t img_height);
	std::string hostname_;
	std::string port_;
	std::size_t img_width_;
	std::size_t img_height_;
};

#endif // __CLIENT_OPTIONS_HPP__
