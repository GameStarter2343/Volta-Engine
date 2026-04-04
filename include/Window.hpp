#pragma once

#include "SDL3/SDL.h"
class Render;
namespace Engine {
    class Window {
    public:
        Window(const char* title, int width, int height, bool fullscreen = false);
        ~Window();

        bool PollEvents(bool& isRunning);
        bool Resize(int& outWidth, int& outHeight);

        int GetWidth() const { return windowSize & 0xFFFF; }
        int GetHeight() const { return (windowSize >> 16) & 0xFFFF; }
        float GetAspect() const { return aspect; }
        SDL_Window* GetWindow() const { return window; }

        void SetWidth(int width);
        void SetHeight(int height);

    private:
        uint32_t windowSize;
        float aspect;
        bool wasResized;

        SDL_Window* window;
        Render* render;

    };
} // namespace Engine
