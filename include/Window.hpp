#pragma once

#include "SDL3/SDL.h"

namespace Engine {
    class Window {
    public:
        Window(const char* title, int width, int height);
        ~Window();
        int GetWidth() const { return w; }
        int GetHeight() const { return h; }
        float GetAspect() const { return aspect; }
        SDL_Window* GetWindow() const { return window; }

        void SetWidth(int width) { w = width; }
        void SetHeight(int height) { h = height; }

    private:
        int w;
        int h;
        float aspect;
        SDL_Window* window;
    };
}
