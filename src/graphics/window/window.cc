#include "window.hh"

#include <GLFW/glfw3.h>

#include "window_desc.hh"

namespace kingom::graphics {
Window::~Window() {
  if (window) {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
}

void Window::apply_hints() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, desc.gl_version_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, desc.gl_version_minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_REFRESH_RATE, desc.refresh_rate);
  glfwWindowHint(GLFW_RESIZABLE, desc.resizable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_DECORATED, desc.borderless ? GLFW_FALSE : GLFW_TRUE);
};

void Window::make_current() { glfwMakeContextCurrent(window); };
bool Window::is_active() { return window == glfwGetCurrentContext(); };

Window::Window(Window&& other) noexcept
    : desc(std::move(other.desc)),
      window(other.window),
      monitor(other.monitor) {
  other.window = nullptr;
  other.monitor = nullptr;
}

Window& Window::operator=(Window&& other) noexcept {
  if (this != &other) {
    if (window) {
      glfwDestroyWindow(window);
    }
    desc = std::move(other.desc);
    window = other.window;
    monitor = other.monitor;
    other.window = nullptr;
    other.monitor = nullptr;
  }
  return *this;
}

util::Result<std::vector<GLFWmonitor*>, std::exception> Window::get_monitors(
    int preference /* = -1 */) {
  int monitor_count = 0;
  GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);

  if (monitor_count == 0) {
    return util::Result<std::vector<GLFWmonitor*>, std::exception>::Err(
        std::runtime_error("Failed to get monitors"));
  }

  std::vector<GLFWmonitor*> result;
  for (int i = 0; i < monitor_count; i++) {
    result.push_back(monitors[i]);
  }

  if (preference >= 0 && preference < monitor_count) {
    std::swap(result[0], result[preference]);
  }

  return util::Result<std::vector<GLFWmonitor*>, std::exception>::Ok(
      std::move(result));
}

util::Result<std::unique_ptr<Window>, std::exception> Window::init(
    const WindowDesc& desc) {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  auto window = std::make_unique<Window>();
  window->desc = desc;
  window->apply_hints();

  auto monitors = window->get_monitors(desc.monitor);
  if (monitors.is_err()) {
    return util::Result<std::unique_ptr<Window>, std::exception>::Err(
        std::runtime_error("Failed to get monitors"));
  }

  window->monitor = monitors.unwrap()[0];
  auto video_mode = glfwGetVideoMode(window->monitor);
  if (!video_mode) {
    return util::Result<std::unique_ptr<Window>, std::exception>::Err(
        std::runtime_error("Failed to get video mode"));
  }

  if (desc.fullscreen) {
    window->window = glfwCreateWindow(video_mode->width, video_mode->height,
                                      desc.title, window->monitor, nullptr);
  } else {
    int width = desc.width;
    int height = desc.height;
    if (desc.width == -1) {
      width = video_mode->width;
    }
    if (desc.height == -1) {
      height = video_mode->height;
    }

    window->window =
        glfwCreateWindow(width, height, desc.title, nullptr, nullptr);
  }

  if (!window->window) {
    return util::Result<std::unique_ptr<Window>, std::exception>::Err(
        std::runtime_error("Failed to create window"));
  }

  window->activate();

  return util::Result<std::unique_ptr<Window>, std::exception>::Ok(
      std::move(window));
}

}  // namespace kingom::graphics