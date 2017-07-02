#include <glclient/signal_dispatcher.hpp>

#include <boost/signals2.hpp>

struct signal_dispatcher::impl {
    boost::signals2::signal<void(double, double)> left_mouse_down_signal;
    boost::signals2::signal<void(double, double)> right_mouse_down_signal;
    boost::signals2::signal<void(double, double)> scroll_signal;
    boost::signals2::signal<void(int, int)> key_down_signal;
    boost::signals2::signal<void(frame const &)> redraw_slm_window_signal;
};

signal_dispatcher::signal_dispatcher(): m_ {new impl()} {}

signal_dispatcher::~signal_dispatcher()
{
    delete m_;
}

void
signal_dispatcher::left_mouse_down(double xpos, double ypos) const
{
    m_->left_mouse_down_signal(xpos, ypos);
}

void
signal_dispatcher::connect_left_mouse_down_slot(
    std::function<void(double, double)> & cb)
{
    m_->left_mouse_down_signal.connect(cb);
}

void
signal_dispatcher::right_mouse_down(double xpos, double ypos) const
{
    m_->right_mouse_down_signal(xpos, ypos);
}

void
signal_dispatcher::connect_right_mouse_down_slot(
    std::function<void(double, double)> & cb)
{
    m_->right_mouse_down_signal.connect(cb);
}

void
signal_dispatcher::scroll(double xoffset, double yoffset) const
{
    m_->scroll_signal(xoffset, yoffset);
}

void
signal_dispatcher::connect_scroll_slot(
    std::function<void(double, double)> & cb)
{
    m_->scroll_signal.connect(cb);
}

void
signal_dispatcher::key_down(int key, int mods) const
{
    m_->key_down_signal(key, mods);
}

void
signal_dispatcher::connect_key_down_slot(
    std::function<void(int, int)> & cb)
{
    m_->key_down_signal.connect(cb);
}

void
signal_dispatcher::redraw_slm_window(frame const & f) const
{
    m_->redraw_slm_window_signal(f);
}

void
signal_dispatcher::connect_redraw_slm_window_slot(
    std::function<void(frame const &)> & cb)
{
    m_->redraw_slm_window_signal.connect(cb);
}

// Local Variables:
// indent-tabs-mode: nil
// End:
