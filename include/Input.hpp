#pragma once

#include <SDL3/SDL.h>
class Input {
public:
    static void Init() {
        keyboardState = SDL_GetKeyboardState(nullptr);
    }

    static bool GetKey(SDL_Scancode key) {
        if (keyboardState) {
            return keyboardState[key];
        }
        return false;
    }

private:
    static const bool* keyboardState;
};

// Определение статического члена
inline const bool* Input::keyboardState = nullptr;
