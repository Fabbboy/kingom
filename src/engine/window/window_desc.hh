#ifndef KINGOM_GRAPHICS_WINDOW_DESC_HH
#define KINGOM_GRAPHICS_WINDOW_DESC_HH

#include <exception>
#include <memory>

#include "util/result.hh"

namespace kingom::engine {
class Window;
struct WindowDesc {
 public:
  int width = 800;
  int height = 600;
  const char* title = "Kingom";
  bool fullscreen = false;
  bool resizable = true;
  int refresh_rate = 60;
  int gl_version_major = 4;
  int gl_version_minor = 6;
  bool borderless = false;
  int monitor = 0;

 public:
  WindowDesc() = default;

  inline WindowDesc& set_width(int w) {
    if (!fullscreen) {
      width = w;
    }
    return *this;
  }

  inline WindowDesc& set_height(int h) {
    if (!fullscreen) {
      height = h;
    }
    return *this;
  }

  inline WindowDesc& set_title(const char* t) {
    title = t;
    return *this;
  }

  inline WindowDesc& set_fullscreen(bool f) {
    fullscreen = f;
    if (f) {
      width = 0;
      height = 0;
    } else {
      width = 800;
      height = 600;
    }
    return *this;
  };

  inline WindowDesc& set_resizable(bool r) {
    resizable = r;
    return *this;
  }

  inline WindowDesc& set_refresh_rate(int r) {
    refresh_rate = r;
    return *this;
  }

  inline WindowDesc& set_gl_version(int major, int minor) {
    gl_version_major = major;
    gl_version_minor = minor;
    return *this;
  }

  inline WindowDesc& set_monitor(int m) {
    monitor = m;
    return *this;
  }

  inline WindowDesc& set_borderless(bool b) {
    borderless = b;
    return *this;
  }

  util::Result<std::shared_ptr<Window>, std::exception> build() const;
};

}  // namespace kingom::engine

#endif  // KINGOM_GRAPHICS_WINDOW_DESC_HH
