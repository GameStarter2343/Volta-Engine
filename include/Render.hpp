#pragma once

struct SDL_Window;

#include "../external/glad/glad.h"
#include <SDL3/SDL.h>
#include <vector>

#include "VMath.hpp"

namespace Engine
{
    class Render {
    public:
        Render(const char* title, int w, int h, bool fullscreen, bool vsync);
        ~Render();

        Render(const Render&) = delete;
        Render& operator=(const Render&) = delete;
        Render(Render&&) = delete;
        Render& operator=(Render&&) = delete;

        void Draw(const std::vector<VMath::Tri3>& tris); // Updated to const reference for performance

        void Poll();
        void Swap();
        void SetClearColor(const VMath::Vec4& color);

        bool IsRunning() const { return isRunning; }
        SDL_Window* GetWindow() const { return window; }

        bool fullscreen;
        bool vsync;


    private:
        bool isRunning;
        SDL_Window* window;
        SDL_GLContext glContext;
        VMath::Vec4 clearColor;

        GLuint m_debugVAO;
        GLuint m_debugVBO;
        GLuint m_debugShaderProgram;

        int w;
        int h;
        float aspect;

        void InitShaders();
        void CleanupRenderer();
        GLuint CompileShader(const char* source, GLenum type);
    };
} // namespace Engine
