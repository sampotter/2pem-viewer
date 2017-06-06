#ifndef __SLM_STATE_HPP__
#define __SLM_STATE_HPP__

#include <boost/optional.hpp>
#include <future>
#include <queue>

#include "options.hpp"
#include "signal_dispatcher.hpp"
#include "frame.hpp"
#include "lens_parameters.hpp"
#include "slm_parameters.hpp"
#include "target_point_store.hpp"

struct slm_state {
    enum class event {
        became_hidden,
        became_visible,
        phase_mask_recomputed
    };

    slm_state(options const & options,
              signal_dispatcher & signal_dispatcher);
    std::vector<target_point> const & get_target_points() const;
    void recompute_phase_mask();
	bool phase_mask_recomputed() const;
    bool visible() const;
	frame get_recomputed_phase_mask();
    boost::optional<event> peek_event() const;
    event pop_event();
private:
    void recompute_target();
    void toggle_visibility();

    // TODO: eventually we want to just get these from some
    // client-local service locator.
    std::size_t img_width_;
    std::size_t img_height_;
    std::size_t gs_iter_count_;
    slm_parameters slm_params_;
    lens_parameters lens_params_;
    bool dirty_ {false};
    bool visible_ {true};

    target_point_store point_store_;
    std::vector<double> source_;
    std::vector<double> target_;

	std::future<frame> recomputed_phase_mask_;

    std::queue<event> event_queue_;
};

#endif // __SLM_STATE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
