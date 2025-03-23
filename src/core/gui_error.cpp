
#include "gui/core/gui_error.h"

namespace gui {

namespace {
error_callback m_error_callback;
} // anonymous namespace

error_callback set_error_callback(error_callback cb) {
  error_callback prev = m_error_callback;
  m_error_callback = cb;
  return prev;
}

void gui_error(int code, const char* format, ...) {
  char desc[256];
  if (format) {
    va_list args;
    va_start(args, format);
    vsnprintf(desc, sizeof(desc), format, args);
    va_end(args);
    desc[sizeof(desc) - 1] = '\0';
  }

  if (m_error_callback) {
    m_error_callback(code, desc);
  } else {
    fprintf(stderr, "GUI Error [%d]: %s\n", code, desc);
  }
}

} // namespace gui
