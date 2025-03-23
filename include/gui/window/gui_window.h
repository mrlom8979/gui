
#pragma once

#include "gui/window/platform/window_impl.h"
#include <memory>
#include <string>

namespace gui::window {

class window {
public:
  window(const std::string& title, int width, int height);
  ~window();

  void pool_events();
  void set_event_callback(const std::function<void(eventhub::event&)>& callback);

private:
  std::unique_ptr<platform::window_impl> impl;
};

} // namespace gui::window
