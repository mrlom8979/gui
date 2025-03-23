
#pragma once

#include <functional>
#include <cstdarg>
#include <cstdio>

namespace gui {

using info_callback = std::function<void(int, const char*)>;

info_callback set_info_callback(info_callback cb);
void gui_info(int code, const char* format, ...);

} // namespace gui
