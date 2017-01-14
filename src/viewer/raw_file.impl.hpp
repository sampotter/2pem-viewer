#ifndef __RAW_FILE_IMPL_HPP__
#define __RAW_FILE_IMPL_HPP__

template <typename elt_t>
raw_file<elt_t>::raw_file(raw_file_params const & params):
    params_ {params},
    mapped_file_ {params_.path}
{}

template <typename elt_t>
std::size_t
raw_file<elt_t>::get_file_size() const
{
    return get_frame_size()*params_.num_chans*params_.time_pts;
}

template <typename elt_t>
std::size_t
raw_file<elt_t>::get_frame_size() const
{
    return params_.dim_x*params_.dim_y;
}

template <typename elt_t>
elt_t const *
raw_file<elt_t>::get_frame_data(std::size_t time_pt, std::size_t chan) const
{
#ifdef VIEWER_DEBUG
    assert(time_pt < params_.time_pts);
    assert(chan < params_.num_chans);
#endif // VIEWER_DEBUG
    auto const addr = reinterpret_cast<elt_t const *>(mapped_file_.data());
    auto const offset = (time_pt*params_.num_chans + chan)*get_frame_size();
    return addr + offset;
}

template <typename elt_t>
std::size_t
raw_file<elt_t>::get_time_pts() const
{
    return params_.time_pts;
}

template <typename elt_t>
std::size_t
raw_file<elt_t>::get_display_chan() const
{
    return params_.display_chan;
}

#endif // __RAW_FILE_IMPL_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
