#ifndef KINGOM_GRAPHICS_RENDERER_HH
#define KINGOM_GRAPHICS_RENDERER_HH
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

namespace kingom::graphics {
class Window;
class Renderer {
 private:
  std::shared_ptr<Window> window;

 public:
  Renderer(std::shared_ptr<Window> window);

  void update();
};
}  // namespace kingom::graphics
#endif