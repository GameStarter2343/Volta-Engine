#include "../include/Input.hpp"

namespace Engine {
    const bool* Input::keyboardState = nullptr;
    bool Input::previousKeyboardState[SDL_SCANCODE_COUNT] = {false};

    Uint32 Input::mouseButtons = 0;
    Uint32 Input::previousMouseButtons = 0;          // ← added (was missing)
    SDL_FPoint Input::mousePosition = {0.0f, 0.0f};

    void Input::Init() {
        SDL_PumpEvents();

        keyboardState = SDL_GetKeyboardState(nullptr);

        memcpy(previousKeyboardState, keyboardState, SDL_SCANCODE_COUNT);

        mouseButtons = SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
        previousMouseButtons = mouseButtons;
    }

    void Input::Update() {
        memcpy(previousKeyboardState, keyboardState, SDL_SCANCODE_COUNT);
        previousMouseButtons = mouseButtons;

        SDL_PumpEvents();

        keyboardState = SDL_GetKeyboardState(nullptr);
        mouseButtons = SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
    }

    void Input::HandleEvent(const SDL_Event& event) {
        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            mousePosition.x = event.motion.x;
            mousePosition.y = event.motion.y;
        }
    }

    bool Input::GetKey(SDL_Scancode key) {
        return keyboardState && keyboardState[key];
    }

    bool Input::GetKeyDown(SDL_Scancode key) {
        return keyboardState &&
               keyboardState[key] &&
               !previousKeyboardState[key];
    }

    bool Input::GetKeyUp(SDL_Scancode key) {
        return keyboardState &&
               !keyboardState[key] &&
               previousKeyboardState[key];
    }

    bool Input::GetMouseButton(Uint8 button) {
        return (mouseButtons & SDL_BUTTON_MASK(button)) != 0;
    }

    bool Input::GetMouseButtonDown(Uint8 button) {
        return (mouseButtons & SDL_BUTTON_MASK(button)) &&
               !(previousMouseButtons & SDL_BUTTON_MASK(button));
    }

    bool Input::GetMouseButtonUp(Uint8 button) {
        return !(mouseButtons & SDL_BUTTON_MASK(button)) &&
               (previousMouseButtons & SDL_BUTTON_MASK(button));
    }

    SDL_FPoint Input::GetMousePosition() {
        return mousePosition;
    }
} // namespace Engine
