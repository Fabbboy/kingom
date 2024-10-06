#ifndef KINGOM_GRAPHICS_CALLBACK_HH
#define KINGOM_GRAPHICS_CALLBACK_HH
#include <memory>

namespace kingom::engine {
class Window;
class Renderer;
namespace internal {
struct CallbackData {
  std::shared_ptr<Window> window;
  std::shared_ptr<Renderer> renderer;

  CallbackData() = default;
  CallbackData(std::shared_ptr<Window> window,
               std::shared_ptr<Renderer> renderer)
      : window(window), renderer(renderer) {}
};
}  // namespace internal
}  // namespace kingom::engine

#endif