#include "engine/rendering/renderer.hh"

#include <iostream>

#include "engine/window/window.hh"

namespace kingom::engine {
Renderer::Renderer(std::shared_ptr<Window> window) : window(window) {
  glfwSetFramebufferSizeCallback(window->get_window(),
                                 framebuffer_size_callback);
}
void Renderer::framebuffer_size_callback(GLFWwindow* window, int width,
                                         int height) {
  auto data =
      static_cast<internal::CallbackData*>(glfwGetWindowUserPointer(window));
  auto win = data->window;
  if (!win) {
    std::cout << "Unable to get window" << std::endl;
  }

  glViewport(0, 0, width, height);
  win->desc.width = width;
  win->desc.height = height;
};
void Renderer::clear() {
  window->activate();
  auto win = window->get_window();
  glClearColor(ccolor.r, ccolor.g, ccolor.b, ccolor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void Renderer::swap() { glfwSwapBuffers(window->get_window()); };
}  // namespace kingom::engine