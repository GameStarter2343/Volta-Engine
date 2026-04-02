#pragma once

#include <SDL3/SDL.h>

namespace Engine {
class Input {
public:
    static void Init();
    static void Update();
    static void HandleEvent(const SDL_Event& event);

    static bool GetKey(SDL_Scancode key);
    static bool GetMouseButton(Uint8 button);
    static SDL_FPoint GetMousePosition();

private:
    static const bool* keyboardState;
    static Uint32 mouseButtons;
    static SDL_FPoint mousePosition;
};

inline const bool* Input::keyboardState = nullptr;
} // namespace Engine
