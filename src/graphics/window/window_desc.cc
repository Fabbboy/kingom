#include "graphics/window/window_desc.hh"

#include "graphics/window/window.hh"

namespace kingom::graphics {
util::Result<std::shared_ptr<Window>, std::exception> WindowDesc::build()
    const {
  return Window::init(*this);
};
}  // namespace kingom::graphics