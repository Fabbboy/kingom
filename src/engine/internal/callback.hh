#ifndef KINGOM_ENGINE_CALLBACK_HH
#define KINGOM_ENGINE_CALLBACK_HH
#include "engine/memory.hh"

namespace kingom::engine {
class Window;
class Renderer;
namespace internal {
struct CallbackData {
  Ref<Window> window;
  Ref<Renderer> renderer;

  CallbackData() = default;
  CallbackData(Ref<Window> window, Ref<Renderer> renderer)
      : window(window), renderer(renderer) {}
};
}  // namespace internal
}  // namespace kingom::engine

#endif