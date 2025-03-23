
#pragma once

#ifdef __WIN32__

#include "gui/window/platform/window_impl.h"
#include <windows.h>
#include <functional>
#include <memory>

namespace gui::window::platform {

class windows_window : public window_impl {
public:
    windows_window();
    ~windows_window();

    void create(const std::string& title, int width, int height) override;
    void destroy() override;
    void pool_events() override;

    void set_event_callback(const std::function<void(eventhub::event&)>& callback) override;

private:
    static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    struct impl;
    std::unique_ptr<impl> pimpl;
};

} // namespace gui::window::platform

#endif // __WIN32__
