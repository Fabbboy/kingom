
#include "engine/window/window.hh"

#include <GLFW/glfw3.h>

#include "engine/rendering/renderer.hh"
#include "engine/window/window_desc.hh"

namespace kingom::engine {
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
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
};

void Window::activate() {
  if (!is_active()) {
    make_current();
  }
};

void Window::make_current() { glfwMakeContextCurrent(window); };
bool Window::is_active() { return window == glfwGetCurrentContext(); };

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

util::Result<Ref<Window>, std::exception> Window::init(const WindowDesc& desc) {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  auto window = make_ref<Window>();
  window->desc = desc;
  window->apply_hints();

  auto monitors = window->get_monitors(desc.monitor);
  if (monitors.is_err()) {
    return util::Result<Ref<Window>, std::exception>::Err(
        std::runtime_error("Failed to get monitors"));
  }

  window->monitor = monitors.unwrap()[0];
  auto video_mode = glfwGetVideoMode(window->monitor);
  if (!video_mode) {
    return util::Result<Ref<Window>, std::exception>::Err(
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
    return util::Result<Ref<Window>, std::exception>::Err(
        std::runtime_error("Failed to create window"));
  }

  window->make_current();

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    return util::Result<Ref<Window>, std::exception>::Err(
        std::runtime_error("Failed to initialize GLEW"));
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  window->renderer = make_ref<Renderer>(window);
  window->callback_data =
      make_ref<internal::CallbackData>(window, window->renderer);

  glfwSetWindowUserPointer(window->window, window->callback_data.get());
  glViewport(0, 0, desc.width, desc.height);

  return util::Result<Ref<Window>, std::exception>::Ok(window);
}

Ref<Renderer> Window::get_renderer() { return renderer; }

}  // namespace kingom::engine