#include "client_options.hpp"

#include <iostream>

#include <boost/program_options.hpp>

client_options
client_options::from_cli_args(int argc, char ** argv)
{
    using namespace boost::program_options;

    options_description desc {"Testing testing"};

    desc.add_options()
        ("help,h", "Display help")
        ("hostname",
         value<std::string>()->default_value("localhost"),
         "The server hostname")
        ("port",
         value<std::string>()->default_value("8888"),
         "The server port")
        ("image_width",
         value<std::size_t>()->default_value(512),
         "The width of the image in pixels")
        ("image_height",
         value<std::size_t>()->default_value(512),
         "The height of the image in pixels")
		("slm_width",
		 value<std::size_t>()->default_value(1024),
		 "The width of the spatial light modulator (SLM) display.")
		("slm_height",
		 value<std::size_t>()->default_value(768),
		 "The height of the SLM display.");

    variables_map varmap;
    store(parse_command_line(argc, argv, desc), varmap);
    notify(varmap);

    if (varmap.count("help")) {
        std::cout << desc << std::endl;
        std::exit(EXIT_FAILURE);
    }

	return client_options {
		varmap["hostname"].as<std::string>(),
		varmap["port"].as<std::string>(),
		varmap["image_width"].as<std::size_t>(),
		varmap["image_height"].as<std::size_t>(),
		varmap["slm_width"].as<std::size_t>(),
		varmap["slm_height"].as<std::size_t>()
	};
}

std::string
client_options::get_hostname() const
{
	return hostname_;
}

std::string
client_options::get_port() const
{
	return port_;
}

std::size_t
client_options::get_img_width() const
{
	return img_width_;
}

std::size_t
client_options::get_img_height() const
{
	return img_height_;
}

std::size_t
client_options::get_slm_width() const
{
	return slm_width_;
}

std::size_t
client_options::get_slm_height() const
{
	return slm_height_;
}

client_options::client_options(std::string const & hostname,
							   std::string const & port,
							   std::size_t img_width,
							   std::size_t img_height,
							   std::size_t slm_width,
							   std::size_t slm_height):
	hostname_ {hostname},
	port_ {port},
	img_width_ {img_width},
	img_height_ {img_height},
	slm_width_ {slm_width},
	slm_height_ {slm_height}
{}
