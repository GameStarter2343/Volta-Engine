#pragma once

#include "../external/glad/glad.h"
#include <SDL3/SDL.h>
#include "VMath.hpp"

#include <unordered_map>
#include <string>
#include <vector>

enum ShaderStage {
    Vertex       = 1 << 0,
    Fragment     = 1 << 1,
    Geometry     = 1 << 2,
    TessControl  = 1 << 3,
    TessEval     = 1 << 4,
    Compute      = 1 << 5
};

namespace Engine
{
    class Render {
    public:
        int w;
        int h;
        float aspect;

        Render(const char* title, int w, int h, bool fullscreen, bool vsync, std::string shaderPath = "external/shaders/basic");
        Render(const char* title, int w, int h, bool fullscreen, bool vsync, std::unordered_map<std::string, uint8_t> shaders);
        ~Render();

        Render(const Render&) = delete;
        Render& operator=(const Render&) = delete;
        Render(Render&&) = delete;
        Render& operator=(Render&&) = delete;

        void Draw(const std::vector<VMath::Tri3>& tris);
        void DrawPoints(const std::vector<VMath::Vec2>& points);

        void Poll();
        void Swap();
        void SetClearColor(const VMath::Vec4& color);
        bool IsRunning() const { return isRunning; }
        SDL_Window* GetWindow() const { return window; }

        void AttachShader(const std::string& programName, const std::string& shaderPath, GLenum type, bool Link = false);
        void SetProgram(const std::string& programName, bool Force = false);
        void SetProgram(GLuint program);

        GLuint GetCurrentProgram() const { return currentProgram; }


        void SetUniform(const std::string& name, int x);
        void SetUniform(const std::string& name, float x);
        void SetUniform(const std::string& name, VMath::Vec2 x);
        void SetUniform(const std::string& name, VMath::Vec3 x);
        void SetUniform(const std::string& name, VMath::Vec4 x);
        void SetUniform(const std::string& name, VMath::m4 x);

        bool fullscreen;
        bool vsync;

    private:
        bool isRunning;
        SDL_Window* window;
        SDL_GLContext glContext;
        VMath::Vec4 clearColor;

        GLuint VAO;
        GLuint VBO;
        GLuint currentProgram;
        std::unordered_map<std::string, GLuint> ShaderPrograms;
        std::unordered_map<std::string, GLint> uniformCache;

        void InitShaders(std::string shaderPath);
        void InitRender(std::string title, bool fullscreen, bool vsync);
        GLuint CompileShader(const char* source, GLenum type);
        GLuint LoadShaderProgram(const std::string& basePath);
    };
} // namespace Engine
