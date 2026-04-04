#pragma once

#include <SDL3/SDL.h>

namespace Engine {
    class Input {
    public:
        static void Init();
        static void Update();
        static void HandleEvent(const SDL_Event& event);

        static bool GetKey(SDL_Scancode key);
        static bool GetKeyDown(SDL_Scancode key);
        static bool GetKeyUp(SDL_Scancode key);

        static bool GetMouseButton(Uint8 button);
        static bool GetMouseButtonDown(Uint8 button);
        static bool GetMouseButtonUp(Uint8 button);

        static SDL_FPoint GetMousePosition();

    private:
        static const bool* keyboardState;

        static bool previousKeyboardState[SDL_SCANCODE_COUNT];

        static Uint32 mouseButtons;
        static Uint32 previousMouseButtons;

        static SDL_FPoint mousePosition;
    };
} // namespace Engine
