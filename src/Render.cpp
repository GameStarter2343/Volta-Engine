#include "../include/Render.hpp"
#include "../include/Debug.hpp"
#include "../external/glad/glad.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_opengl.h>
#include <stdexcept> // For std::runtime_error

namespace Engine
{
    Render::Render(const char* title, int w, int h)
        : window(nullptr)
        , glContext(nullptr)
        , clearColor(0.1f, 0.1f, 0.1f, 1.0f)
        , w(w)
        , h(h)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;
        window = SDL_CreateWindow(title, w, h, flags);

        if (!window) {
            throw std::runtime_error(std::string("Failed to create SDL Window: ") + SDL_GetError());
        }

        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            throw std::runtime_error(std::string("Failed to create OpenGL Context: ") + SDL_GetError());
        }

        int version = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
        if (version == 0) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        int major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        SDL_Log("OpenGL Version: %d.%d", major, minor);

        SetupOpenGLState();

        SDL_ShowWindow(window);
    }

    Render::~Render() {
        if (glContext) { SDL_GL_DestroyContext(glContext); glContext = nullptr; }
        if (window) { SDL_DestroyWindow(window); window = nullptr; }
    }

    void Render::BeginFrame() {
        if (!window) return;

        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Render::EndFrame() {
        if (!window) return;
        SDL_GL_SwapWindow(window);
    }

    void Render::OnResize(int newW, int newH) {
        w = newW;
        h = newH;
        glViewport(0, 0, w, h);
    }

    void Render::SetClearColor(const VMath::Vec4& color) {
        clearColor = color;
    }

    void Render::SetupOpenGLState() {
        glViewport(0, 0, w, h);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}
