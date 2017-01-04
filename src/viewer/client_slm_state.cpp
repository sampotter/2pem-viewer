#include "client_slm_state.hpp"

#include <functional>
#include <iostream>

#include "phase_retrieval.hpp"
#include "glfw.hpp"
#include "utility.hpp"

client_slm_state::client_slm_state(
    client_options const & options,
    client_signal_dispatcher & signal_dispatcher):
    img_width_ {options.get_img_width()},
    img_height_ {options.get_img_height()},
    slm_width_ {options.get_slm_width()},
    slm_height_ {options.get_slm_height()},
    gs_iter_count_ {options.get_gs_iter_count()},
    source_(img_width_*img_height_, 1.0),
    target_(img_width_*img_height_, 0.0)
{
    {
        auto func = std::function<void(double, double)> {
            [this] (double x, double y) {
                point_store_.add_target_point(x, y);
            }
        };
        signal_dispatcher.connect_left_mouse_down_slot(func);
    }

    {
        auto func = std::function<void(double, double)> {
            [this] (double x, double y) {
                point_store_.remove_target_point(x, y);
            }
        };
        signal_dispatcher.connect_right_mouse_down_slot(func);
    }

    {
        auto func = std::function<void(double, double)> {
            [] (double /* xoffset */, double yoffset) {
                auto const r = target_point::screen_axicon_radius;
                auto const new_r = std::max(5.0, r + yoffset/10.0);
                target_point::screen_axicon_radius = new_r;
            }
        };
        signal_dispatcher.connect_scroll_slot(func);
    }

    {
        auto func = std::function<void(int, int)> {
            [this] (int key, int /* mods */) {
                if (key == GLFW_KEY_SPACE) {
                    recompute_phase_mask();
                } else if (key == GLFW_KEY_V) {
                    toggle_visibility();
                }
            }
        };
        signal_dispatcher.connect_key_down_slot(func);
    }
}

std::vector<target_point> const &
client_slm_state::get_target_points() const
{
    return point_store_.get_target_points();
}

void
client_slm_state::recompute_phase_mask()
{
    recomputed_phase_mask_ = std::async(
        std::launch::async,
        [this] () {
            recompute_target();
            frame phase_mask {slm_width_, slm_height_};
            phase_retrieval::compute_phase_mask(
                &source_[0],
                &target_[0],
                img_width_,
                img_height_,
                slm_width_,
                slm_height_,
                gs_iter_count_,
                phase_mask);
            phase_retrieval::apply_axicon_phase_mask(
                target_point::screen_axicon_radius,
                phase_mask);

            event_queue_.push(event::phase_mask_recomputed);

            return phase_mask;
        });
}

bool
client_slm_state::visible() const
{
    return visible_;
}

frame
client_slm_state::get_recomputed_phase_mask()
{
    return recomputed_phase_mask_.get();
}

boost::optional<client_slm_state::event>
client_slm_state::peek_event() const
{
    boost::optional<event> tmp;
    if (!event_queue_.empty()) {
        tmp = event_queue_.front();
    }
    return tmp;
}

client_slm_state::event
client_slm_state::pop_event()
{
    auto const tmp = event_queue_.front();
    event_queue_.pop();
    return tmp;
}

void
client_slm_state::recompute_target()
{
    memset((char *) &target_[0], 0x0, sizeof(double)*target_.size());
    for (auto const & pt: get_target_points()) {
        target_[pt.get_y()*img_width_ + pt.get_x()] = 1.0;
    }
}

void
client_slm_state::toggle_visibility()
{
    event_queue_.push(
        (visible_ = !visible_) ? event::became_visible : event::became_hidden);
}

// Local Variables:
// indent-tabs-mode: nil
// End:
