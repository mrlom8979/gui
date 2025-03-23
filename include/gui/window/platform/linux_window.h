
#pragma once

#ifdef __LINUX__

#include "gui/window/platform/window_impl.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <functional>
#include <memory>

namespace gui::window::platform {

class linux_window : public window_impl {
public:
    linux_window();
    ~linux_window();

    void create(const std::string& title, int width, int height) override;
    void destroy() override;
    void pool_events() override;

    void set_event_callback(const std::function<void(eventhub::event&)>& callback) override;

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

} // namespace gui::window::platform

#endif // __LINUX__
