#include "graphics/renderer.hh"

#include "graphics/window/window.hh"

namespace kingom::graphics {
Renderer::Renderer(std::shared_ptr<Window> window) : window(window) {}

void Renderer::update() {
  window->activate();
  auto win = window->get_window();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glfwSwapBuffers(win);
};
}  // namespace kingom::graphics