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

    void Render::InitShaders(std::string shaderPath) {
        Debug::Log("Initializing Shaders", 3); Debug::LogSpace(3);
        std::ifstream vertF(shaderPath + ".vert");
        std::ifstream fragF(shaderPath + ".frag");
        std::ifstream geomF(shaderPath + ".geom");
        if (!vertF.is_open()) {
            Debug::Log("ERROR: Failed to open vert shader: " + shaderPath + ".vert", 1);
            return;
        }
        if (!fragF.is_open()) {
            Debug::Log("ERROR: Failed to open frag shader: " + shaderPath + ".frag", 1);
            return;
        }
        if (!geomF.is_open()) {
            Debug::Log("ERROR: Failed to open geom shader: " + shaderPath + ".geom", 1);
            return;
        }
        std::string vertSource((std::istreambuf_iterator<char>(vertF)), std::istreambuf_iterator<char>());
        std::string fragSource((std::istreambuf_iterator<char>(fragF)), std::istreambuf_iterator<char>());
        std::string geomSource((std::istreambuf_iterator<char>(geomF)), std::istreambuf_iterator<char>());

        vertF.close();
        fragF.close();
        geomF.close();

        GLuint vertShader = CompileShader(vertSource.c_str(), GL_VERTEX_SHADER);
        GLuint fragShader = CompileShader(fragSource.c_str(), GL_FRAGMENT_SHADER);
        GLuint geomShader = CompileShader(geomSource.c_str(), GL_GEOMETRY_SHADER);

        ShaderProgram = glCreateProgram();
        glAttachShader(ShaderProgram, vertShader);
        glAttachShader(ShaderProgram, fragShader);
        glAttachShader(ShaderProgram, geomShader);
        glLinkProgram(ShaderProgram);

        int success;
        char infoLog[512];
        glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ShaderProgram, 512, nullptr, infoLog);
            Debug::Log("FATAL: Shader Linking Failed: " + std::string(infoLog), 1); Debug::LogSpace(1);
            throw std::runtime_error(std::string("FATAL: Shader Linking Failed: ") + infoLog);
        }

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        Debug::Log("Shaders Initialized Successfully", 2);
    }

    void Render::CleanupRenderer() {
        Debug::Log("Cleanup Renderer", 2);
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (ShaderProgram) glDeleteProgram(ShaderProgram);
    }

    Render::Render(const char* title, int w, int h, bool fullscreen, bool vsync, std::string shaderPath)
        : window(nullptr), isRunning(true), glContext(nullptr)
        , clearColor(0.1f, 0.1f, 0.17f, 1.0f), w(w), h(h), aspect(float(w)/float(h))
        , VAO(0), VBO(0), ShaderProgram(0)
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
            throw std::runtime_error("FATAL: Failed to initialize GLAD"); Debug::LogSpace(1);
        }

        glViewport(0, 0, w, h);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        InitShaders(shaderPath);
        Debug::Log("Successfully Initialized Renderer", 1);
        Debug::Log("Width: " + std::to_string(w) + ", Height: " + std::to_string(h), 2); Debug::LogSpace(2);
        Debug::Log("fullscreen: " + std::to_string(fullscreen ? 1 : 0), 2); Debug::LogSpace(2);
        Debug::Log("vsync: " + std::to_string(vsync ? 1 : 0), 2); Debug::LogSpace(2); Debug::LogSpace(2);
    }

    void Render::Draw(const std::vector<VMath::Tri3>& tris) {
        if (tris.empty()) return;

        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
    void Render::AttachShader(const std::string& shaderPath, GLenum type) {
        std::ifstream file(shaderPath);
        if (!file.is_open()) {
            Debug::Log("ERROR: Failed to open shader file: " + shaderPath, 1);
            return;
        }
        std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        GLuint shader = CompileShader(source.c_str(), type);
        glAttachShader(ShaderProgram, shader);
        glLinkProgram(ShaderProgram);
        glDeleteShader(shader);
    }

    void Render::SetUniform(const std::string& name, int x) {
        glUseProgram(ShaderProgram);
        GLint loc = glGetUniformLocation(ShaderProgram, name.c_str());
        if (loc != -1) glUniform1i(loc, x); }
    void Render::SetUniform(const std::string& name, float x) {
        glUseProgram(ShaderProgram);
        GLint loc = glGetUniformLocation(ShaderProgram, name.c_str());
        if (loc != -1) glUniform1f(loc, x); }
    void Render::SetUniform(const std::string& name, VMath::Vec2 x) {
        glUseProgram(ShaderProgram);
        GLint loc = glGetUniformLocation(ShaderProgram, name.c_str());
        if (loc != -1) glUniform2f(loc, x.x, x.y); }
    void Render::SetUniform(const std::string& name, VMath::Vec3 x) {
        glUseProgram(ShaderProgram);
        GLint loc = glGetUniformLocation(ShaderProgram, name.c_str());
        if (loc != -1) glUniform3f(loc, x.x, x.y, x.z); }
    void Render::SetUniform(const std::string& name, VMath::Vec4 x) {
        glUseProgram(ShaderProgram);
        GLint loc = glGetUniformLocation(ShaderProgram, name.c_str());
        if (loc != -1) glUniform4f(loc, x.x, x.y, x.z, x.w); }
    void Render::SetUniform(const std::string& name, VMath::m4 x) {
        glUseProgram(ShaderProgram);
        GLint loc = glGetUniformLocation(ShaderProgram, name.c_str());
        if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, x.data()); }

    void Render::Swap() { if (!window) return; SDL_GL_SwapWindow(window); }
    void Render::SetClearColor(const VMath::Vec4& color) { clearColor = color; }
}
