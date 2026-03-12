#include "../include/Render.hpp"
#include "../include/Debug.hpp"
#include "../external/glad/glad.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

namespace Engine
{
    Render::Render(const char* title, int w, int h, bool fullscreen, bool vsync)
        : window(nullptr), isRunning(true), glContext(nullptr)
        , clearColor(0.1f, 0.1f, 0.17f, 1.0f), w(w), h(h)
        {
            if (SDL_Init(SDL_INIT_VIDEO) == false) {
                throw std::runtime_error(SDL_GetError());
            }

            if (fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            if (vsync) SDL_GL_SetSwapInterval(1);

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            window = SDL_CreateWindow(title, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
            if (!window) { throw std::runtime_error(SDL_GetError()); }

            glContext = SDL_GL_CreateContext(window);
            if (!glContext) { throw std::runtime_error(SDL_GetError()); }

            if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) { throw std::runtime_error("Failed to initialize GLAD"); }

            glViewport(0, 0, w, h);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }

    void Render::BeginFrame() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                OnResize(event.window.data1, event.window.data2);
            }
        }

        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    Render::~Render() { if (glContext) { SDL_GL_DestroyContext(glContext); glContext = nullptr; } if (window) { SDL_DestroyWindow(window); window = nullptr; } }
    void Render::EndFrame() { if (!window) return; SDL_GL_SwapWindow(window); }
    void Render::OnResize(int newW, int newH) { w = newW; h = newH; glViewport(0, 0, w, h); }
    void Render::SetClearColor(const VMath::Vec4& color) { clearColor = color; }
}
