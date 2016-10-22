#ifndef __RAW_FILE_HPP__
#define __RAW_FILE_HPP__

#include <memory>
#include <string>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

struct raw_file_params {
    std::string path;
    std::size_t time_pts {15000};
    std::size_t dim_x {512};
    std::size_t dim_y {512};
    std::size_t num_chans {4};
    std::size_t display_chan {1};
};

template <typename elt_t>
struct raw_file {
    raw_file(raw_file_params const & params);
    std::size_t get_file_size() const;
    std::size_t get_frame_size() const;
    elt_t const * get_frame_data(std::size_t time_pt, std::size_t chan) const;
    std::size_t get_time_pts() const;
    std::size_t get_display_chan() const;
private:
    raw_file_params params_;
    boost::interprocess::file_mapping file_mapping_;
    boost::interprocess::mapped_region mapped_region_;
};

#include "raw_file.impl.hpp"

#endif // __RAW_FILE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
