
#include "gui/core/gui_info.h"

namespace gui {

namespace {
info_callback m_info_callback;
} // anonymous namespace

info_callback set_info_callback(info_callback cb) {
  info_callback prev = m_info_callback;
  m_info_callback = cb;
  return prev;
}

void gui_info(int code, const char* format, ...) {
  char desc[256];
  if (format) {
    va_list args;
    va_start(args, format);
    vsnprintf(desc, sizeof(desc), format, args);
    va_end(args);
    desc[sizeof(desc) - 1] = '\0';
  }

  if (m_info_callback) {
    m_info_callback(code, desc);
  } else {
    fprintf(stderr, "GUI Info [%d]: %s\n", code, desc);
  }
}

} // namespace gui
