#ifndef KINGOM_ENGINE_INPUT_HH
#define KINGOM_ENGINE_INPUT_HH

#include <unordered_map>

#include "engine/input/keycode.hh"
#include "engine/memory.hh"

namespace kingom::engine {
class Window;
class InputStates {
 private:
  std::unordered_map<KeyCode, InputState> keys;
  std::unordered_map<KeyCode, InputState> prev_keys;
  std::unordered_map<MouseButton, InputState> mouse_buttons;
  std::unordered_map<MouseButton, InputState> prev_mouse_buttons;

  CursorPosition cursor_position;
  CursorScroll scroll;

 private:
  static void key_callback_handler(GLFWwindow* window, int key, int scancode,
                                   int action, int mods);

  static void mouse_button_callback_handler(GLFWwindow* window, int button,
                                            int action, int mods);

  static void cursor_position_callback_handler(GLFWwindow* window, double xpos,
                                               double ypos);

  static void scroll_callback_handler(GLFWwindow* window, double xoffset,
                                      double yoffset);

 public:
  InputStates(Ref<Window> window);
  void set_key(KeyCode key, InputState state);
  void set_mouse_button(MouseButton button, InputState state);
  InputState get_key(KeyCode key) const;
  InputState get_prev_key(KeyCode key) const;
  InputState get_mouse_button(MouseButton button) const;
  InputState get_prev_mouse_button(MouseButton button) const;
  void update();
};

class Input {
 private:
  InputStates states;

 public:
  Input(Ref<Window> window);
  bool is_key_pressed(KeyCode key) const;
  bool is_key_released(KeyCode key) const;
  bool is_key_repeat(KeyCode key) const;
  bool is_mouse_button_pressed(MouseButton button) const;
  bool is_mouse_button_released(MouseButton button) const;
  bool is_mouse_button_repeat(MouseButton button) const;
  bool is_key_down(KeyCode key) const;
  bool is_mouse_button_down(MouseButton button) const;
  bool is_key_up(KeyCode key) const;
  bool is_mouse_button_up(MouseButton button) const;
  void __internal__update();
  InputStates& __internal__get_states();
};

}  // namespace kingom::engine

#endif
