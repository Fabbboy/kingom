#include "window_desc.hh"

#include "window.hh"

namespace kingom::graphics {
util::Result<std::unique_ptr<Window>, std::exception> WindowDesc::build() const {
  return Window::init(*this);
};
}  // namespace kingom::graphics