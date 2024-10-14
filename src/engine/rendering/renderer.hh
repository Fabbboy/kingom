#ifndef KINGOM_ENGINE_RENDERER_HH
#define KINGOM_ENGINE_RENDERER_HH
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "engine/memory.hh"

namespace kingom::engine {
class Window;
class Renderer {
 private:
  Ref<Window> window;
  glm::vec4 ccolor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);

 private:
  static void framebuffer_size_callback(GLFWwindow* window, int width,
                                        int height);

 public:
  Renderer(Ref<Window> window);

  void swap();
  void clear();
  inline void set_ccolor(glm::vec4 color) { ccolor = color; };
};
}  // namespace kingom::engine
#endif