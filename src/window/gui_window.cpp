
#include "gui/window/gui_window.h"

#ifdef __LINUX__
#include "gui/window/platform/linux_window.h"
#elif defined(__WIN32__)
#include "gui/window/platform/windows_window.h"
#elif defined(__APPLE__)
#include "gui/window/platform/macos_window.h"
#endif

namespace gui::window {

window::window(const std::string& title, int width, int height) {
#ifdef __linux__
  impl = std::make_unique<platform::linux_window>();
#elif defined(_WIN32)
  impl = std::make_unique<platform::windows_window>();
#elif defined(__APPLE__)
  impl = std::make_unique<platform::macos_window>();
#endif
  impl->create(title, width, height);
}

window::~window() {
  impl->destroy();
}

void window::pool_events() {
  impl->pool_events();
}

void window::set_event_callback(const std::function<void(eventhub::event&)>& callback) {
  impl->set_event_callback(callback);
}

} // namespace gui::window
