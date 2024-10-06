#include <iostream>
#include <sstream>

#include "graphics/rendering/shader.hh"
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

  std::stringstream vertexShader(
      R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    void main() {
      gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
  )");

  std::stringstream fragmentShader(
      R"(
    #version 460 core
    out vec4 FragColor;
    void main() {
      FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
  )");

  auto shader = kingom::graphics::Shader::create(vertexShader, fragmentShader);

  if (shader.is_err()) {
    std::cerr << "Failed to create shader: " << shader.unwrap_err().what()
              << std::endl;
    return -1;
  }

  while (!window->should_close()) {
    window->poll_events();
    renderer->clear();
    renderer->swap();
  }

  return 0;
}
