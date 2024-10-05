#ifndef KINGOM_GRAPHICS_WINDOW_HH
#define KINGOM_GRAPHICS_WINDOW_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

#include "graphics/renderer.hh"
#include "util/result.hh"
#include "window_desc.hh"

#define MAX -1

namespace kingom::graphics {
class Window : public std::enable_shared_from_this<Window> {
 private:
  WindowDesc desc;
  GLFWwindow* window;
  GLFWmonitor* monitor;

 private:
  void apply_hints();
  void make_current();
  bool is_active();

  util::Result<std::vector<GLFWmonitor*>, std::exception> get_monitors(
      int preference = -1);

  inline std::shared_ptr<Window> get_this() { return shared_from_this(); }

 public:
  Window() = default;
  ~Window();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  Window(Window&& other) noexcept;
  Window& operator=(Window&& other) noexcept;

  static util::Result<std::shared_ptr<Window>, std::exception> init(
      const WindowDesc& desc);

  Renderer get_renderer();

  inline void activate() {
    if (!is_active()) {
      make_current();
    }
  }

  inline GLFWwindow* get_window() { return window; }

  inline void poll_events() { glfwPollEvents(); }
  inline bool should_close() { return glfwWindowShouldClose(window); }
  inline void close() { glfwSetWindowShouldClose(window, GLFW_TRUE); }
};
}  // namespace kingom::graphics

#endif  // KINGOM_GRAPHICS_WINDOW_HH