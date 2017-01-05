#include <cstdio>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "../external/dir_monitor/include/dir_monitor/dir_monitor.hpp"

struct options {
	std::string path;
	std::string hostname;
	std::string port;
	std::size_t image_width;
	std::size_t image_height;
};

/**
 * Parse command-line options and return populated options struct.
 */
options parse_args(int argc, char ** argv)
{
	using namespace boost::program_options;

	/**
	 * Create and populate options description object.
	 */
	options_description desc {"Testing testing"};
	desc.add_options()
		("help,h", "Display help")
		("path", value<std::string>(), "The directory")
		("hostname", value<std::string>()->default_value("localhost"),
		 "The server hostname")
		("port", value<std::string>()->default_value("8888"),
		 "The server port")
        ("image_width", value<std::size_t>()->default_value(512),
         "The width of the image in pixels")
        ("image_height", value<std::size_t>()->default_value(512),
         "The height of the image in pixels");

	/**
	 * Populate variables map and print options description if help
	 * flag is passed.
	 */
	variables_map varmap;
	store(parse_command_line(argc, argv, desc), varmap);
	notify(varmap);
	if (varmap.count("help")) {
		std::cout << desc << std::endl;
		std::exit(EXIT_FAILURE);
	}

	/**
	 * Fill out options struct and return.
	 */
	options opts;
	opts.path = varmap["path"].as<std::string>();
	opts.hostname = varmap["hostname"].as<std::string>();
	opts.port = varmap["port"].as<std::string>();
	opts.image_width = varmap["image_width"].as<std::size_t>();
	opts.image_height = varmap["image_height"].as<std::size_t>();
	return opts;
}

void handle_event(boost::asio::dir_monitor & dm,
				  boost::system::error_code const & ec,
				  boost::asio::dir_monitor_event const & ev)
{
	using namespace boost::asio;
	using namespace boost::system;

	if (ec) {
		std::cerr << "Error: " << ec << std::endl;
	} else {
		std::cout << "Event: " << ev << std::endl;

		auto const handler = [&] (error_code const & error_code,
								  dir_monitor_event const & event) {
			handle_event(dm, ec, ev);
		};
		dm.async_monitor(handler);
	}
}

int main(int argc, char ** argv)
{
	using namespace boost::asio;
	using namespace boost::system;

	auto const opts = parse_args(argc, argv);

	io_service io;
	dir_monitor dm {io};
	dm.add_directory(opts.path);
	auto const handler = [&] (error_code const & error_code,
							  dir_monitor_event const & event) {
		handle_event(dm, error_code, event);
	};
	dm.async_monitor(handler);
	io_service::work workload {io};
	io.run();
}
