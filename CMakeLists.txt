cmake_minimum_required(VERSION 3.16)
project(gui)

# Переключатель для выбора типа библиотеки (по умолчанию статическая)
option(GUI_BUILD_SHARED "Build gui as a shared library" OFF)

# Поиск всех .cpp и .h файлов
file(GLOB_RECURSE GUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE GUI_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Платформо-зависимые исходные файлы
if (WIN32)
  list(APPEND GUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/window/platform/windows_window.cpp")
elseif (UNIX AND NOT APPLE)
  list(APPEND GUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/window/platform/linux_window.cpp")
elseif (APPLE)
  # Добавьте здесь файлы для macOS, если они есть
endif()

# Поиск библиотеки X11 (только для Linux)
if(UNIX AND NOT APPLE)
  find_package(X11 REQUIRED)
  if (X11_FOUND)
    message(STATUS "X11 found: ${X11_LIBRARIES}")
  else()
    message(FATAL_ERROR "X11 not found! Please install X11 development libraries.")
  endif()
endif()

# Создание библиотеки
if (GUI_BUILD_SHARED)
  add_library(gui SHARED ${GUI_SOURCES} ${GUI_HEADERS})
  target_compile_definitions(gui PRIVATE GUI_SHARED)
else()
  add_library(gui STATIC ${GUI_SOURCES} ${GUI_HEADERS})
endif()

# Добавление предкомпилированных заголовков (если используется pch.h)
# target_precompile_headers(gui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")

# Установка директорий для заголовочных файлов
target_include_directories(gui PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/vendor/eventhub/include
)

# Линковка X11 (только для Linux)
if(UNIX AND NOT APPLE)
  target_link_libraries(gui ${X11_LIBRARIES})
endif()

# Настройка платформо-зависимых флагов
if(UNIX AND NOT APPLE)
  target_compile_definitions(gui PRIVATE __LINUX__)
elseif (WIN32)
  target_compile_definitions(gui PRIVATE __WIN32__)
elseif (APPLE)
  target_compile_definitions(gui PRIVATE __MACOS__)
endif()
