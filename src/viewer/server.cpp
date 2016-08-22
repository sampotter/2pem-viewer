#include <cassert>
#include <chrono>
#include <cinttypes>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/optional.hpp>

struct options {
	int port;
	std::string rawfile;
};

struct raw_file {
	raw_file(std::string path):
		file_mapping_ {path.c_str(), boost::interprocess::read_only},
		mapped_region_ {
			file_mapping_, 
			boost::interprocess::read_only,
			0,
			get_file_size()}
	{}
				
	std::size_t get_file_size() const {
		return get_frame_size()*num_chans*time_pts;
	}

	std::size_t get_frame_size() const {
		return dim_x*dim_y;
	}

	uint16_t const * get_frame_data(std::size_t time_pt, std::size_t chan) const {
#ifdef VIEWER_DEBUG
		assert(time_pt < time_pts);
		assert(chan < num_chans);
#endif // VIEWER_DEBUG
		auto const addr = static_cast<uint16_t *>(mapped_region_.get_address());
		auto const offset = (time_pt*num_chans + chan)*get_frame_size();
		return addr + offset;
	}

	std::size_t time_pts {15000};
	std::size_t dim_x {512};
	std::size_t dim_y {512};
	std::size_t num_chans {4};
	std::size_t display_chan {1};
	std::size_t elt_bytes {sizeof(uint16_t)};
	
private:
	boost::interprocess::file_mapping file_mapping_;
	boost::interprocess::mapped_region mapped_region_;
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

	raw_file raw_file {options->rawfile};

	{
		using namespace boost::asio;
		using namespace boost::asio::ip;
		using namespace boost::system;

		io_service io;
		tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), options->port));

		auto time_pts = raw_file.time_pts;
		auto chan = raw_file.display_chan;
		
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
