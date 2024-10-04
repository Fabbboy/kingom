#ifndef KINGOM_GRAPHICS_WINDOW_HH
#define KINGOM_GRAPHICS_WINDOW_HH

#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

#include "util/result.hh"
#include "window_desc.hh"

#define MAX -1

namespace kingom::graphics {
class Window {
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

 public:
  Window() = default;
  ~Window();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  Window(Window&& other) noexcept;
  Window& operator=(Window&& other) noexcept;

  static util::Result<std::unique_ptr<Window>, std::exception> init(
      const WindowDesc& desc);

  inline void activate() {
    if (!is_active()) {
      make_current();
    }
  }

  inline void poll_events() { glfwPollEvents(); }
  inline void swap_buffers() { glfwSwapBuffers(window); }
  inline bool should_close() { return glfwWindowShouldClose(window); }
  inline void close() { glfwSetWindowShouldClose(window, GLFW_TRUE); }
  inline void clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  }
};
}  // namespace kingom::graphics

#endif  // KINGOM_GRAPHICS_WINDOW_HH