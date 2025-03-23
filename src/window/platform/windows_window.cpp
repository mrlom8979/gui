
#ifdef __WIN32__

#include "gui/window/platform/windows_window.h"
#include "gui/core/gui_error.h"
#include "gui/core/gui_info.h"
#include <eventhub.h>
#include <iostream>

namespace gui::window::platform {

struct windows_window::impl {
  HWND hwnd = nullptr;
  std::function<void(eventhub::event&)> event_callback;
};

windows_window::windows_window() : pimpl(std::make_unique<impl>()) {}

windows_window::~windows_window() {
  destroy();
}

void windows_window::create(const std::string& title, int width, int height) {
  HINSTANCE hInstance = GetModuleHandle(nullptr);

  // Регистрация класса окна
  WNDCLASSEX wc = {};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = windows_window::window_proc;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.lpszClassName = "GUIWindowClass";

  if (!RegisterClassEx(&wc)) {
    gui::gui_error(1, "Failed to register window class");
    return;
  }

  // Создание окна
  pimpl->hwnd = CreateWindowEx(
    0,
    "GUIWindowClass",
    title.c_str(),
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, width, height,
    nullptr,
    nullptr,
    hInstance,
    this // Передаем указатель на текущий объект
  );

  if (!pimpl->hwnd) {
    gui::gui_error(1, "Failed to create window");
    return;
  }

  // Показываем окно
  ShowWindow(pimpl->hwnd, SW_SHOW);
  UpdateWindow(pimpl->hwnd);

  gui::gui_info(0, "Windows window created: %s", title.c_str());
}

void windows_window::destroy() {
  if (pimpl->hwnd) {
    DestroyWindow(pimpl->hwnd);
    pimpl->hwnd = nullptr;
    gui::gui_info(0, "Windows window destroyed");
  }
}

void windows_window::pool_events() {
  MSG msg;
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void windows_window::set_event_callback(const std::function<void(eventhub::event&)>& callback) {
  pimpl->event_callback = callback;
}

LRESULT CALLBACK windows_window::window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  windows_window* window = nullptr;

  if (msg == WM_NCCREATE) {
    CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lparam);
    window = static_cast<windows_window*>(create->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
  } else {
    window = reinterpret_cast<windows_window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  }

  if (window) {
    switch (msg) {
      case WM_CLOSE: {
        eventhub::window_close_event e;
        window->pimpl->event_callback(e);
        return 0;
      }
      case WM_SIZE: {
        int width = LOWORD(lparam);
        int height = HIWORD(lparam);
        eventhub::window_resize_event e(width, height);
        window->pimpl->event_callback(e);
        break;
      }
      case WM_KEYDOWN: {
        eventhub::key_pressed_event e(static_cast<int>(wparam), 0);
        window->pimpl->event_callback(e);
        break;
      }
      case WM_KEYUP: {
        eventhub::key_released_event e(static_cast<int>(wparam));
        window->pimpl->event_callback(e);
        break;
      }
      case WM_LBUTTONDOWN: {
        eventhub::mouse_button_pressed_event e(1); // Левая кнопка мыши
        window->pimpl->event_callback(e);
        break;
      }
      case WM_LBUTTONUP: {
        eventhub::mouse_button_released_event e(1); // Левая кнопка мыши
        window->pimpl->event_callback(e);
        break;
      }
      case WM_MOUSEMOVE: {
        int x = GET_X_LPARAM(lparam);
        int y = GET_Y_LPARAM(lparam);
        eventhub::mouse_moved_event e(x, y);
        window->pimpl->event_callback(e);
        break;
      }
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

} // namespace gui::window::platform

#endif // __WIN32__
