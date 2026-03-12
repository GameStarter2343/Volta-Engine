#pragma once

struct SDL_Window;

#include "../external/glad/glad.h"
#include <SDL3/SDL.h>
#include <string>

#include "Volta-Math.hpp"

namespace Engine
{
    class Render
    {
    public:
        Render(const char* title, int w, int h);
        ~Render();

        Render(const Render&) = delete;
        Render& operator=(const Render&) = delete;
        Render(Render&&) = delete;
        Render& operator=(Render&&) = delete;

        void Shutdown();
        void BeginFrame();
        void EndFrame();
        void OnResize(int newW, int newH);
        void SetClearColor(const VMath::Vec4& color);

        bool IsRunning() const { return isRunning; }
        SDL_Window* GetWindow() const { return window; }

    private:
        bool isRunning;
        SDL_Window* window;
        SDL_GLContext glContext;
        VMath::Vec4 clearColor;
        int w;
        int h;

        void SetupOpenGLState();
    };
} // namespace Engine
