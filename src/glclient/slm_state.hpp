#ifndef __SLM_STATE_HPP__
#define __SLM_STATE_HPP__

#include <dsp/frame.hpp>
#include <glclient/options.hpp>
#include <glclient/signal_dispatcher.hpp>
#include <glclient/target_point.hpp>

struct slm_state {
    enum class event {
        became_hidden,
        became_visible,
        phase_mask_recomputed,
        none
    };
    slm_state(options const & options, signal_dispatcher & signal_dispatcher);
    ~slm_state();
    std::vector<target_point> const & get_target_points() const;
    void recompute_phase_mask();
    bool visible() const;
	frame get_recomputed_phase_mask();
    event peek_event() const;
    event pop_event();
private:
    slm_state();
    struct impl;
    impl * m_;
};

#endif // __SLM_STATE_HPP__

// Local Variables:
// indent-tabs-mode: nil
// End:
