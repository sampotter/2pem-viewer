#ifndef __CLIENT_SLM_STATE_HPP__
#define __CLIENT_SLM_STATE_HPP__

#include <future>

#include "client_options.hpp"
#include "client_signal_dispatcher.hpp"
#include "frame.hpp"
#include "target_point_store.hpp"

struct client_slm_state {
    client_slm_state(client_options const & options,
                     client_signal_dispatcher & signal_dispatcher);
    std::vector<target_point> const & get_target_points() const;
    void recompute_phase_mask();
	bool phase_mask_recomputed() const;
	frame get_recomputed_phase_mask();
private:
    void recompute_target();

    // TODO: eventually we want to just get these from some
    // client-local service locator.
    std::size_t img_width_;
    std::size_t img_height_;
    std::size_t slm_width_;
    std::size_t slm_height_;
    std::size_t gs_iter_count_;

    target_point_store point_store_;
    std::vector<double> source_;
    std::vector<double> target_;

	std::future<frame> recomputed_phase_mask_;
};

#endif // __CLIENT_SLM_STATE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
