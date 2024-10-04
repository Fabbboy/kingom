#include <iostream>

#include "graphics/window/window.hh"
#include "graphics/window/window_desc.hh"

int main() {
  auto result = kingom::graphics::WindowDesc()
                    .set_fullscreen(false)
                    .set_width(800)
                    .set_height(600)
                    .build();

  if (result.is_err()) {
    std::cerr << "Failed to create window: " << result.unwrap_err().what()
              << std::endl;
    return -1;
  }

  auto window = result.unwrap();

  while (!window->should_close()) {
    window->poll_events();
    window->clear();
    window->swap_buffers();
  }

  std::cout << "Exiting program..." << std::endl;
  return 0;
}
