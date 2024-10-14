#include "engine/window/window_desc.hh"

#include "engine/window/window.hh"

namespace kingom::engine {
util::Result<Ref<Window>, std::exception> WindowDesc::build() const {
  return Window::init(*this);
};
}  // namespace kingom::engine