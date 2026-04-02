#pragma once

#include "SDL3/SDL.h"

namespace Engine {
class Window {
public:
    Window(const char* title, int width, int height, bool fullscreen = false);
    ~Window();

    bool PollEvents(bool& isRunning);
    bool Resize(int& outWidth, int& outHeight);

    int GetWidth() const { return w; }
    int GetHeight() const { return h; }
    float GetAspect() const { return aspect; }
    SDL_Window* GetWindow() const { return window; }

    void SetWidth(int width);
    void SetHeight(int height);

private:
    int w;
    int h;
    float aspect;
    bool wasResized;
    SDL_Window* window;
};
} // namespace Engine
