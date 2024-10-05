#include <iostream>

#include "graphics/window/window.hh"
#include "graphics/window/window_desc.hh"

int main() {
  auto result =
      kingom::graphics::WindowDesc().set_width(MAX).set_height(MAX).build();

  if (result.is_err()) {
    std::cerr << "Failed to create window: " << result.unwrap_err().what()
              << std::endl;
    return -1;
  }

  auto window = result.unwrap();
  auto renderer = window->get_renderer();

  while (!window->should_close()) {
    window->poll_events();
    renderer.update();
  }

  return 0;
}
