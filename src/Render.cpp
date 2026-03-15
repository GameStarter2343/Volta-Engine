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
    void Render::InitRender(std::string title, bool fullscreen, bool vsync) {
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

        window = SDL_CreateWindow(title.c_str(), w, h, flags);
        if (!window) {
            Debug::Log("FATAL: " + std::string(SDL_GetError()), 1);
            throw std::runtime_error("FATAL: " + std::string(SDL_GetError()));
        }
        if (vsync) SDL_GL_SetSwapInterval(1);

        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            Debug::Log("FATAL: " + std::string(SDL_GetError()), 1);
            throw std::runtime_error("FATAL: " + std::string(SDL_GetError()));
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            Debug::Log("FATAL: Failed to initialize GLAD", 1);
            throw std::runtime_error("FATAL: Failed to initialize GLAD");
        }

        glViewport(0, 0, w, h);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
    Render::Render(const char* title, int w, int h, bool fullscreen, bool vsync, std::string shaderName)
        : window(nullptr), isRunning(true), glContext(nullptr)
        , clearColor(0.1f, 0.1f, 0.17f, 1.0f), w(w), h(h), aspect(float(w)/float(h))
        , VAO(0), VBO(0)
    {
        InitRender(title, fullscreen, vsync);
        CreateProgram("basic");
        AttachShader("basic", shaderName + ".vert", GL_VERTEX_SHADER);
        AttachShader("basic", shaderName + ".frag", GL_FRAGMENT_SHADER);
        AttachShader("basic", shaderName + ".geom", GL_GEOMETRY_SHADER);
        Debug::Log("Successfully Initialized Renderer", 1);
        Debug::Log("Width: " + std::to_string(w) + ", Height: " + std::to_string(h), 2);
        Debug::Log("fullscreen: " + std::to_string(fullscreen ? 1 : 0), 2);
        Debug::Log("vsync: " + std::to_string(vsync ? 1 : 0), 2);
    }

    void Render::Draw(const std::vector<VMath::Tri3>& tris) {
        if (tris.empty()) return;

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, tris.size() * sizeof(VMath::Tri3), tris.data(), GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(tris.size() * 3));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Render::Poll() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
                Debug::Log("EVENT: SDL Quit Event Called", 1);
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                w = event.window.data1;
                h = event.window.data2;
                glViewport(0, 0, w, h);
                Debug::Log("EVENT: Window resized: " + std::to_string(w) + "x" + std::to_string(h), 3);
            }
        }

        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    Render::~Render() {
        Debug::Log("Cleanup Renderer", 2);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        for (auto const& [name, program] : ShaderPrograms) glDeleteProgram(program);

        if (window && glContext) {
            SDL_GL_MakeCurrent(window, NULL);
            SDL_GL_DestroyContext(glContext);
            glContext = nullptr;
        }

        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        ShaderPrograms.clear();

        Debug::Log("Render Destructed", 2);
    }

    void Render::AttachShader(const std::string& programName, const std::string& shaderPath, GLenum type) {
        auto program = ShaderPrograms.find(programName);
        Debug::Log("Attaching Shader: " + shaderPath, 2);
        if (program == ShaderPrograms.end()) {
            Debug::Log("ERROR: Program not found: " + programName, 1);
            return;
        }

        std::ifstream file(shaderPath);
        if (!file.is_open()) {
            Debug::Log("ERROR: Failed to open file: " + shaderPath, 1);
            return;
        }
        std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        const char* srcPtr = source.c_str();

        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &srcPtr, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            glDeleteShader(shader);
            throw std::runtime_error("Shader Compilation Failed: " + std::string(infoLog));
        }

        glAttachShader(program->second, shader);
        glDeleteShader(shader);

        glLinkProgram(program->second);
    }

    void Render::CreateProgram(const std::string& programName) {
        ShaderPrograms[programName] = glCreateProgram();
        glUseProgram(ShaderPrograms[programName]);
        currentProgram = ShaderPrograms[programName];
    }

    void Render::SetProgram(const std::string& programName) {
        if (ShaderPrograms.find(programName) != ShaderPrograms.end()) {
            glUseProgram(ShaderPrograms[programName]);
            currentProgram = ShaderPrograms[programName];
        }
        else Debug::Log("ERROR: Program not found: " + programName, 1);
    }
    void Render::SetUniform(const std::string& name, int x) {
        GLint loc = glGetUniformLocation(currentProgram, name.c_str());
        if (loc != -1) glUniform1i(loc, x);
        else Debug::Log("ERROR: Uniform not found: " + name, 1); }
    void Render::SetUniform(const std::string& name, float x) {
        GLint loc = glGetUniformLocation(currentProgram, name.c_str());
        if (loc != -1) glUniform1f(loc, x);
        else Debug::Log("ERROR: Uniform not found: " + name, 1); }
    void Render::SetUniform(const std::string& name, VMath::Vec2 x) {
        GLint loc = glGetUniformLocation(currentProgram, name.c_str());
        if (loc != -1) glUniform2f(loc, x.x, x.y);
        else Debug::Log("ERROR: Uniform not found: " + name, 1); }
    void Render::SetUniform(const std::string& name, VMath::Vec3 x) {
        GLint loc = glGetUniformLocation(currentProgram, name.c_str());
        if (loc != -1) glUniform3f(loc, x.x, x.y, x.z);
        else Debug::Log("ERROR: Uniform not found: " + name, 1); }
    void Render::SetUniform(const std::string& name, VMath::Vec4 x) {
        GLint loc = glGetUniformLocation(currentProgram, name.c_str());
        if (loc != -1) glUniform4f(loc, x.x, x.y, x.z, x.w);
        else Debug::Log("ERROR: Uniform not found: " + name, 1); }
    void Render::SetUniform(const std::string& name, VMath::m4 x) {
        GLint loc = glGetUniformLocation(currentProgram, name.c_str());
        if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, x.data());
        else Debug::Log("ERROR: Uniform not found: " + name, 1); }

    void Render::Swap() { if (!window) return; SDL_GL_SwapWindow(window); }
    void Render::SetClearColor(const VMath::Vec4& color) { clearColor = color; }
}
