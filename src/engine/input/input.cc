#include "engine/window/window.hh"
// window.hh needs to stay here

#include <GL/glext.h>

#include <iostream>

#include "engine/input/input.hh"
#include "engine/internal/callback.hh"

namespace kingom::engine {

void InputStates::key_callback_handler(GLFWwindow* window, int key,
                                       int scancode, int action, int mods) {
  auto cbdata =
      static_cast<internal::CallbackData*>(glfwGetWindowUserPointer(window));
  auto input = cbdata->input;
  if (input == nullptr) {
    std::cout << "Unable to get input" << std::endl;
    return;
  }

  auto& states = input->__internal__get_states();
  auto key_code = static_cast<KeyCode>(key);

  if (action == GLFW_PRESS) {
    states.set_key(key_code, InputState::Pressed);
  } else if (action == GLFW_RELEASE) {
    states.set_key(key_code, InputState::Released);
  } else if (action == GLFW_REPEAT) {
    states.set_key(key_code, InputState::Repeat);
  }
};

void InputStates::mouse_button_callback_handler(GLFWwindow* window, int button,
                                                int action, int mods) {
  auto cbdata =
      static_cast<internal::CallbackData*>(glfwGetWindowUserPointer(window));

  auto input = cbdata->input;

  if (input == nullptr) {
    std::cout << "Unable to get input" << std::endl;
    return;
  }

  auto& states = input->__internal__get_states();
  auto mouse_button = static_cast<MouseButton>(button);

  if (action == GLFW_PRESS) {
    states.set_mouse_button(mouse_button, InputState::Pressed);
  } else if (action == GLFW_RELEASE) {
    states.set_mouse_button(mouse_button, InputState::Released);
  } else if (action == GLFW_REPEAT) {
    states.set_mouse_button(mouse_button, InputState::Repeat);
  }
};

void InputStates::cursor_position_callback_handler(GLFWwindow* window,
                                                   double xpos, double ypos) {
  auto cbdata =
      static_cast<internal::CallbackData*>(glfwGetWindowUserPointer(window));

  auto input = cbdata->input;

  if (input == nullptr) {
    std::cout << "Unable to get input" << std::endl;
    return;
  }

  auto& states = input->__internal__get_states();
  states.cursor_position = {xpos, ypos};
};

void InputStates::scroll_callback_handler(GLFWwindow* window, double xoffset,
                                          double yoffset) {
  auto cbdata =
      static_cast<internal::CallbackData*>(glfwGetWindowUserPointer(window));

  auto input = cbdata->input;

  if (input == nullptr) {
    std::cout << "Unable to get input" << std::endl;
    return;
  }

  auto& states = input->__internal__get_states();
  states.scroll = {xoffset, yoffset};
};

InputStates::InputStates(Ref<Window> window) {
  glfwSetKeyCallback(window->get_window(), key_callback_handler);
}

void InputStates::set_key(KeyCode key, InputState state) { keys[key] = state; }

void InputStates::set_mouse_button(MouseButton button, InputState state) {
  mouse_buttons[button] = state;
}

InputState InputStates::get_key(KeyCode key) const {
  auto it = keys.find(key);
  return it != keys.end() ? it->second : InputState::Released;
}

InputState InputStates::get_prev_key(KeyCode key) const {
  auto it = prev_keys.find(key);
  return it != prev_keys.end() ? it->second : InputState::Released;
}

InputState InputStates::get_mouse_button(MouseButton button) const {
  auto it = mouse_buttons.find(button);
  return it != mouse_buttons.end() ? it->second : InputState::Released;
}

InputState InputStates::get_prev_mouse_button(MouseButton button) const {
  auto it = prev_mouse_buttons.find(button);
  return it != prev_mouse_buttons.end() ? it->second : InputState::Released;
}

void InputStates::update() {
  prev_keys = keys;
  prev_mouse_buttons = mouse_buttons;
}

Input::Input(Ref<Window> window) : states(window) {}

bool Input::is_key_pressed(KeyCode key) const {
  return states.get_key(key) == InputState::Pressed;
}

bool Input::is_key_released(KeyCode key) const {
  return states.get_key(key) == InputState::Released;
}

bool Input::is_key_repeat(KeyCode key) const {
  return states.get_key(key) == InputState::Repeat;
}

bool Input::is_mouse_button_pressed(MouseButton button) const {
  return states.get_mouse_button(button) == InputState::Pressed;
}

bool Input::is_mouse_button_released(MouseButton button) const {
  return states.get_mouse_button(button) == InputState::Released;
}

bool Input::is_mouse_button_repeat(MouseButton button) const {
  return states.get_mouse_button(button) == InputState::Repeat;
}

bool Input::is_key_down(KeyCode key) const {
  return is_key_pressed(key) || is_key_repeat(key);
}

bool Input::is_mouse_button_down(MouseButton button) const {
  return is_mouse_button_pressed(button) || is_mouse_button_repeat(button);
}

bool Input::is_key_up(KeyCode key) const { return is_key_released(key); }

bool Input::is_mouse_button_up(MouseButton button) const {
  return is_mouse_button_released(button);
}

void Input::__internal__update() { states.update(); }

InputStates& Input::__internal__get_states() { return states; }

}  // namespace kingom::engine
