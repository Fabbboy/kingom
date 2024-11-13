#ifndef KINGOM_ENGINE_CALLBACK_HH
#define KINGOM_ENGINE_CALLBACK_HH
#include "engine/input/input.hh"
#include "engine/memory.hh"

namespace kingom::engine {
class Window;
class Renderer;
namespace internal {
struct CallbackData {
  Ref<Window> window;
  Ref<Renderer> renderer;
  Ref<Input> input;

  CallbackData() = default;
  CallbackData(Ref<Window> window, Ref<Renderer> renderer, Ref<Input> input)
      : window(window), renderer(renderer), input(input) {}
};
}  // namespace internal
}  // namespace kingom::engine

#endif