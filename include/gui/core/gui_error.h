
#pragma once

#include <functional>
#include <cstdarg>
#include <cstdio>

namespace gui {

using error_callback = std::function<void(int, const char*)>;

error_callback set_error_callback(error_callback cb);
void gui_error(int code, const char* format, ...);

} // namespace gui
