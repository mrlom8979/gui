
#ifdef __LINUX__

#include "gui/window/platform/linux_window.h"
#include "gui/core/gui_error.h"
#include "gui/core/gui_info.h"
#include <eventhub.h>
#include <iostream>

namespace gui::window::platform {

struct linux_window::impl {
    Display* display = nullptr;
    Window window = 0;
    std::function<void(eventhub::event&)> event_callback;
};

linux_window::linux_window() : pimpl(std::make_unique<impl>()) {}

linux_window::~linux_window() {
  destroy();
}

void linux_window::create(const std::string& title, int width, int height) {
  pimpl->display = XOpenDisplay(nullptr);
  if (!pimpl->display) {
    gui::gui_error(1, "Failed to open X11 display");
    return;
  }

  int screen = DefaultScreen(pimpl->display);
  Window root = RootWindow(pimpl->display, screen);

  pimpl->window = XCreateSimpleWindow(
    pimpl->display, root,
    0, 0, width, height, 1,
    BlackPixel(pimpl->display, screen),
    WhitePixel(pimpl->display, screen)
  );

  XStoreName(pimpl->display, pimpl->window, title.c_str());
  XSelectInput(pimpl->display, pimpl->window, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);

  XMapWindow(pimpl->display, pimpl->window);
  XFlush(pimpl->display);

  gui::gui_info(0, "Linux window created: %s", title.c_str());
}

void linux_window::destroy() {
  if (pimpl->display && pimpl->window) {
    XDestroyWindow(pimpl->display, pimpl->window);
    XCloseDisplay(pimpl->display);
    pimpl->display = nullptr;
    pimpl->window = 0;
    gui::gui_info(0, "Linux window destroyed");
  }
}

void linux_window::pool_events() {
  if (!pimpl->display) return;

  XEvent event;
  while (XPending(pimpl->display)) {
    XNextEvent(pimpl->display, &event);

    switch (event.type) {
      case Expose:
        // Обработка события перерисовки
        break;
      case KeyPress: {
        eventhub::key_pressed_event e(event.xkey.keycode, 0);
        pimpl->event_callback(e);
        break;
      }
      case KeyRelease: {
        eventhub::key_released_event e(event.xkey.keycode);
        pimpl->event_callback(e);
        break;
      }
      case ButtonPress: {
        eventhub::mouse_button_pressed_event e(event.xbutton.button);
        pimpl->event_callback(e);
        break;
      }
      case ButtonRelease: {
        eventhub::mouse_button_released_event e(event.xbutton.button);
        pimpl->event_callback(e);
        break;
      }
      case MotionNotify: {
        eventhub::mouse_moved_event e(event.xmotion.x, event.xmotion.y);
        pimpl->event_callback(e);
        break;
      }
      case ConfigureNotify: {
        eventhub::window_resize_event e(event.xconfigure.width, event.xconfigure.height);
        pimpl->event_callback(e);
        break;
      }
      case ClientMessage: {
        // Обработка закрытия окна
        if (event.xclient.data.l[0] == XInternAtom(pimpl->display, "WM_DELETE_WINDOW", False)) {
          eventhub::window_close_event e;
          pimpl->event_callback(e);
        }
        break;
      }
    }
  }
}

void linux_window::set_event_callback(const std::function<void(eventhub::event&)>& callback) {
  pimpl->event_callback = callback;

  // Устанавливаем обработчик закрытия окна
  Atom wm_delete_window = XInternAtom(pimpl->display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(pimpl->display, pimpl->window, &wm_delete_window, 1);
}

} // namespace gui::window::platform

#endif // __LINUX__

