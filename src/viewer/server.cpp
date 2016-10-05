#include <cassert>
#include <chrono>
#include <cinttypes>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/optional.hpp>

#include "raw_file.hpp"

struct options {
	int port;
	std::string rawfile;
};

boost::optional<options>
parse_command_line_options(int argc, char ** argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <port> <rawfile>\n", argv[0]);
		return boost::none;
	}
	options opts;
	opts.port = std::stoi(std::string(argv[1]));
	opts.rawfile = std::string(argv[2]);
	return opts;
}

auto now() {
	return std::chrono::system_clock::now();
}

int main(int argc, char ** argv) {
	auto const options = parse_command_line_options(argc, argv);
	if (!options) {
		std::exit(EXIT_FAILURE);
	}

	raw_file<uint16_t> raw_file {{.path = options->rawfile}};

	{
		using namespace boost::asio;
		using namespace boost::asio::ip;
		using namespace boost::system;

		io_service io;
		tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), options->port));

		auto time_pts = raw_file.get_time_pts();
		auto chan = raw_file.get_display_chan();
		
		tcp::socket socket(io);
		acceptor.accept(socket);
		fprintf(stdout, "Accepted connection\n");

		auto const frame_size = raw_file.get_frame_size();
		std::vector<float> frame(frame_size);
			
		for (decltype(time_pts) time_pt {0}; time_pt < time_pts; ++time_pt) {
			fprintf(stdout, "Sending frame %lu...", time_pt);
			
			auto const data = raw_file.get_frame_data(time_pt, chan);
			for (std::size_t i {0}; i < frame_size; ++i) {
				frame[i] = static_cast<float>(data[i])/
					static_cast<float>(std::numeric_limits<uint16_t>::max());
			}
			auto const num_frame_bytes = sizeof(float)*frame.size();
			
			error_code error_code;
			auto const len = boost::asio::write(
				socket,
				buffer((void *) &frame[0], num_frame_bytes),
				error_code);
			fprintf(stdout, " wrote %lu bytes (expected %lu)\n",
					len, num_frame_bytes);
			if (error_code) {
				fprintf(stderr, "error: %s\n", error_code.message().c_str());
				std::exit(EXIT_FAILURE);
			}
		}
	}
}
