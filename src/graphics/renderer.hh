#ifndef KINGOM_GRAPHICS_RENDERER_HH
#define KINGOM_GRAPHICS_RENDERER_HH
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <memory>

namespace kingom::graphics {
class Window;
class Renderer {
 private:
  std::shared_ptr<Window> window;
  glm::vec4 ccolor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);

 private:
  static void framebuffer_size_callback(GLFWwindow* window, int width,
                                        int height);

 public:
  Renderer(std::shared_ptr<Window> window);

  void swap();
  void update();
};
}  // namespace kingom::graphics
#endif