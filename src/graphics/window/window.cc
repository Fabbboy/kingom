#include "window.hh"

#include <GLFW/glfw3.h>

#include <iostream>

#include "window_desc.hh"

namespace kingom::graphics {
Window::~Window() {
  if (window) {
    std::cout << "Destroying window..." << std::endl;
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
  glfwWindowHint(GLFW_RESIZABLE, desc.resizable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_REFRESH_RATE, desc.refresh_rate);
};

static void error_callback(int error, const char* description) {
  std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
  // Instead of throwing an exception, handle it here.
}

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

util::Result<std::unique_ptr<Window>, std::exception> Window::init(
    const WindowDesc& desc) {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  auto window_instance = std::make_unique<Window>();
  window_instance->desc = desc;
  window_instance->apply_hints();

  int monitor_count = 0;
  GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);

  window_instance->monitor = monitors[desc.monitor];
  if (!window_instance->monitor) {
    return util::Result<std::unique_ptr<Window>, std::exception>::Err(
        std::runtime_error("Failed to get monitor"));
  }

  if (desc.fullscreen) {
    const GLFWvidmode* mode = glfwGetVideoMode(window_instance->monitor);
    if (!mode) {
      return util::Result<std::unique_ptr<Window>, std::exception>::Err(
          std::runtime_error("Failed to get video mode"));
    }
    window_instance->window =
        glfwCreateWindow(mode->width, mode->height, desc.title,
                         window_instance->monitor, nullptr);
  } else {
    window_instance->window =
        glfwCreateWindow(desc.width, desc.height, desc.title, nullptr, nullptr);
  }

  if (!window_instance->window) {
    return util::Result<std::unique_ptr<Window>, std::exception>::Err(
        std::runtime_error("Failed to create window"));
  }

  glfwMakeContextCurrent(window_instance->window);

  return util::Result<std::unique_ptr<Window>, std::exception>::Ok(
      std::move(window_instance));
}

}  // namespace kingom::graphics