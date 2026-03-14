#include "../include/Render.hpp"
#include "../include/Debug.hpp"
#include "../external/glad/glad.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <string>
#include <vector>
#include <stdexcept>

namespace Engine
{
    GLuint Render::CompileShader(const char* source, GLenum type) {
        Debug::Log("Compiling Shader: ", 3); Debug::Log(source, 3); Debug::LogSpace(3);
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            Debug::Log("FATAL: Compiling Shader Failed: " + std::string(infoLog), 1); Debug::LogSpace(1);
            throw std::runtime_error(std::string("FATAL: Shader Compilation Failed: ") + infoLog);
        }
        return shader;
    }

    void Render::InitShaders() {
        Debug::Log("Initializing Shaders", 3); Debug::LogSpace(3);

        const char* vertSource = R"(
            #version 450 core
            layout (location = 0) in vec3 aPos;
            void main() {
                gl_Position = vec4(aPos, 1.0);
            }
        )";

        const char* fragSource = R"(
            #version 450 core
            out vec4 FragColor;
            void main() {
                FragColor = vec4(1.0, 1.0, 1.0, 1.0);
            }
        )";

        GLuint vertShader = CompileShader(vertSource, GL_VERTEX_SHADER);
        GLuint fragShader = CompileShader(fragSource, GL_FRAGMENT_SHADER);

        m_debugShaderProgram = glCreateProgram();
        glAttachShader(m_debugShaderProgram, vertShader);
        glAttachShader(m_debugShaderProgram, fragShader);
        glLinkProgram(m_debugShaderProgram);

        int success;
        char infoLog[512];
        glGetProgramiv(m_debugShaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_debugShaderProgram, 512, nullptr, infoLog);
            Debug::Log("FATAL: Shader Linking Failed: " + std::string(infoLog), 1); Debug::LogSpace(1);
            throw std::runtime_error(std::string("FATAL: Shader Linking Failed: ") + infoLog);
        }

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        glGenVertexArrays(1, &m_debugVAO);
        glGenBuffers(1, &m_debugVBO);

        glBindVertexArray(m_debugVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_debugVBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        Debug::Log("Shaders Initialized Successfully", 2);
    }

    void Render::CleanupRenderer() {
        Debug::Log("Cleanup Renderer", 2);
        if (m_debugVAO) glDeleteVertexArrays(1, &m_debugVAO);
        if (m_debugVBO) glDeleteBuffers(1, &m_debugVBO);
        if (m_debugShaderProgram) glDeleteProgram(m_debugShaderProgram);
    }

    Render::Render(const char* title, int w, int h, bool fullscreen, bool vsync)
        : window(nullptr), isRunning(true), glContext(nullptr)
        , clearColor(0.1f, 0.1f, 0.17f, 1.0f), w(w), h(h), aspect(float(w)/float(h))
        , m_debugVAO(0), m_debugVBO(0), m_debugShaderProgram(0)
    {

        if (SDL_Init(SDL_INIT_VIDEO) == false) {
            Debug::Log("FATAL: Failed to initialize SDL", 1);
            throw std::runtime_error(SDL_GetError());
        }
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
        if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

        window = SDL_CreateWindow(title, w, h, flags);
        if (!window) {
            Debug::Log(1, 1);
            throw std::runtime_error(SDL_GetError());
        }

        if (vsync) SDL_GL_SetSwapInterval(1);

        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            Debug::Log(-1, 1);
            throw std::runtime_error(SDL_GetError());
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            Debug::Log(1, 1);
            throw std::runtime_error("FATAL: Failed to initialize GLAD"); Debug::LogSpace(1);
        }

        glViewport(0, 0, w, h);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        InitShaders();
        Debug::Log("Successfully Initialized Renderer", 1);
        Debug::Log("Width: " + std::to_string(w) + ", Height: " + std::to_string(h), 2); Debug::LogSpace(2);
        Debug::Log("fullscreen: " + std::to_string(fullscreen ? 1 : 0), 2); Debug::LogSpace(2);
        Debug::Log("vsync: " + std::to_string(vsync ? 1 : 0), 2); Debug::LogSpace(2); Debug::LogSpace(2);
    }

    void Render::Draw(const std::vector<VMath::Tri3>& tris) {
        if (tris.empty()) return;

        Debug::Log(static_cast<int>(tris.size()), 3);

        glUseProgram(m_debugShaderProgram);
        glBindVertexArray(m_debugVAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_debugVBO);
        glBufferData(GL_ARRAY_BUFFER, tris.size() * sizeof(VMath::Tri3), tris.data(), GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(tris.size() * 3));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void Render::Poll() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
                Debug::Log("FATAL: SDL Quit Event Called", 1); Debug::LogSpace(1);
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                w = event.window.data1;
                h = event.window.data2;
                glViewport(0, 0, w, h);
                Debug::Log("EVENT: Window resized: " + std::to_string(w) + "x" + std::to_string(h), 3); Debug::LogSpace(3);
            }
        }

        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    Render::~Render() {
        CleanupRenderer();
        if (glContext) { SDL_GL_DestroyContext(glContext); glContext = nullptr; }
        if (window) { SDL_DestroyWindow(window); window = nullptr; }
        Debug::Log("Render Destructed", 2); Debug::LogSpace(2);
    }

    void Render::Swap() { if (!window) return; SDL_GL_SwapWindow(window); }
    void Render::SetClearColor(const VMath::Vec4& color) { clearColor = color; }
}
