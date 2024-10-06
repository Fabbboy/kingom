#ifndef KINGOM_GRAPHICS_WINDOW_HH
#define KINGOM_GRAPHICS_WINDOW_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

#include "engine/callback.hh"
#include "engine/rendering/renderer.hh"
#include "util/result.hh"
#include "window_desc.hh"

#define MAX -1

namespace kingom::engine {
class Window {
  friend class Renderer;

 private:
  WindowDesc desc;
  GLFWwindow* window;
  GLFWmonitor* monitor;
  std::shared_ptr<internal::CallbackData> callback_data;
  std::shared_ptr<Renderer> renderer;

 private:
  void apply_hints();
  void make_current();
  bool is_active();

  util::Result<std::vector<GLFWmonitor*>, std::exception> get_monitors(
      int preference = -1);

  std::shared_ptr<internal::CallbackData> get_callback_data() {
    return callback_data;
  }

 public:
  Window() = default;
  ~Window();

  static util::Result<std::shared_ptr<Window>, std::exception> init(
      const WindowDesc& desc);

  std::shared_ptr<Renderer> get_renderer();

  void activate();

  inline GLFWwindow* get_window() { return window; }
  inline std::pair<int, int> get_size() {
    return std::make_pair(desc.width, desc.height);
  }

  inline void poll_events() { glfwPollEvents(); }
  inline bool should_close() { return glfwWindowShouldClose(window); }
  inline void close() { glfwSetWindowShouldClose(window, GLFW_TRUE); }
};
}  // namespace kingom::engine

#endif  // KINGOM_GRAPHICS_WINDOW_HH