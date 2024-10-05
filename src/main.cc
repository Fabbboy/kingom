#include <iostream>

#include "graphics/window/window.hh"
#include "graphics/window/window_desc.hh"

static void glfwErrorCallback(int error, const char* description) {
  std::cerr << "GLFW Error: " << description << std::endl;
}

int main() {
  glfwSetErrorCallback(glfwErrorCallback);
  auto result =
      kingom::graphics::WindowDesc().set_width(800).set_height(600).build();

  if (result.is_err()) {
    std::cerr << "Failed to create window: " << result.unwrap_err().what()
              << std::endl;
    return -1;
  }

  auto window = result.unwrap();
  auto renderer = window->get_renderer();

  while (!window->should_close()) {
    window->poll_events();
    renderer->update();
    renderer->swap();
  }

  return 0;
}
