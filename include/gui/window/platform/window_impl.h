
#pragma once

#include <string>
#include <functional>
#include <eventhub.h>

namespace gui::window::platform {

class window_impl {
public:
  virtual ~window_impl() = default;

  virtual void create(const std::string& title, int width, int height) = 0;
  virtual void destroy() = 0;
  virtual void pool_events() = 0;

  virtual void set_event_callback(const std::function<void(eventhub::event&)>& callback) = 0;
};

} // namespace gui::window::platform
