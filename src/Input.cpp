#include "../include/Input.hpp"

namespace Engine {
Uint32 Input::mouseButtons = 0;
SDL_FPoint Input::mousePosition = {0.0f, 0.0f};

void Input::Init() {
    keyboardState = SDL_GetKeyboardState(nullptr);
    mouseButtons = SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
}

void Input::Update() {
    if (!keyboardState) {
        keyboardState = SDL_GetKeyboardState(nullptr);
    }

    mouseButtons = SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
}

void Input::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        mousePosition.x = event.motion.x;
        mousePosition.y = event.motion.y;
    }
}

bool Input::GetKey(SDL_Scancode key) {
    if (!keyboardState) {
        return false;
    }

    return keyboardState[key];
}

bool Input::GetMouseButton(Uint8 button) {
    return (mouseButtons & SDL_BUTTON_MASK(button)) != 0;
}

SDL_FPoint Input::GetMousePosition() {
    return mousePosition;
}
} // namespace Engine
