#ifndef __CLIENT_SLM_STATE_HPP__
#define __CLIENT_SLM_STATE_HPP__

#include <boost/optional.hpp>
#include <future>
#include <queue>

#include "client_options.hpp"
#include "client_signal_dispatcher.hpp"
#include "frame.hpp"
#include "target_point_store.hpp"

struct client_slm_state {
    enum class event {
        became_hidden,
        became_visible,
        phase_mask_recomputed
    };

    client_slm_state(client_options const & options,
                     client_signal_dispatcher & signal_dispatcher);
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
    std::size_t slm_width_;
    std::size_t slm_height_;
    std::size_t gs_iter_count_;
    bool dirty_ {false};
    bool visible_ {true};

    target_point_store point_store_;
    std::vector<double> source_;
    std::vector<double> target_;

	std::future<frame> recomputed_phase_mask_;

    std::queue<event> event_queue_;
};

#endif // __CLIENT_SLM_STATE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
