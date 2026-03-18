#include "../include/Render.hpp"
#include "../include/Debug.hpp"
#include "../external/glad/glad.h"
#include <SDL3/SDL.h>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>
namespace Engine
{
    static std::string programUniformKey(GLuint program, const std::string& name) { return std::to_string(program) + ":" + name; }
    void Render::InitRender(std::string title, bool fullscreen, bool vsync) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            Debug::Log("FATAL: Failed to initialize SDL", 1);
            throw std::runtime_error(SDL_GetError());
        }
        lastFrameTime = SDL_GetTicks();
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

        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            Debug::Log("FATAL: " + std::string(SDL_GetError()), 1);
            throw std::runtime_error("FATAL: " + std::string(SDL_GetError()));
        }
        if (vsync) SDL_GL_SetSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            Debug::Log("FATAL: Failed to initialize GLAD", 1);
            throw std::runtime_error("FATAL: Failed to initialize GLAD");
        }

        glViewport(0, 0, w, h);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VMath::Vec3), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Render::Render(const char* title, int w, int h, bool fullscreen, bool vsync, std::string shaderName)
        : window(nullptr), isRunning(true), glContext(nullptr)
        , clearColor(0.1f, 0.1f, 0.17f, 1.0f), w(w), h(h), aspect(float(w)/float(h))
        , fullscreen(fullscreen), vsync(vsync), VAO(0), VBO(0), currentProgram(0)
    {
        InitRender(title, fullscreen, vsync);
        SetProgram("basic", true);
        if (std::filesystem::exists("external/shaders/" + shaderName + ".vert")) AttachShader("basic", "external/shaders/" + shaderName + ".vert", GL_VERTEX_SHADER);
        if (std::filesystem::exists("external/shaders/" + shaderName + ".frag")) AttachShader("basic", "external/shaders/" + shaderName + ".frag", GL_FRAGMENT_SHADER);
        if (std::filesystem::exists("external/shaders/" + shaderName + ".frag")) AttachShader("basic", "external/shaders/" + shaderName + ".geom", GL_GEOMETRY_SHADER);
        if (std::filesystem::exists("external/shaders/" + shaderName + ".tesc")) AttachShader("basic", "external/shaders/" + shaderName + ".tesc", GL_TESS_CONTROL_SHADER);
        if (std::filesystem::exists("external/shaders/" + shaderName + ".tese")) AttachShader("basic", "external/shaders/" + shaderName + ".tese", GL_TESS_EVALUATION_SHADER);
        if (std::filesystem::exists("external/shaders/" + shaderName + ".comp")) AttachShader("basic", "external/shaders/" + shaderName + ".comp", GL_COMPUTE_SHADER);

        glLinkProgram(ShaderPrograms["basic"]);
        GLint isLinked = false;
        glGetProgramiv(ShaderPrograms["basic"], GL_LINK_STATUS, &isLinked);
        if (!isLinked) {
            char log[256];
            glGetProgramInfoLog(ShaderPrograms["basic"], 256, nullptr, log);
            Debug::Log("Shader program linking failed: " + std::string(log), 0);
        }
        Debug::Log("Successfully Initialized Renderer", 1);
        Debug::Log("Width: " + std::to_string(w) + ", Height: " + std::to_string(h), 2);
        Debug::Log("fullscreen: " + std::to_string(fullscreen ? true : false), 2);
        Debug::Log("vsync: " + std::to_string(vsync ? true : false), 2);
    }

    Render::Render(const char* title, int w, int h, bool fullscreen, bool vsync, std::unordered_map<std::string, uint8_t> shaders)
        : window(nullptr), isRunning(true), glContext(nullptr)
        , clearColor(0.1f, 0.1f, 0.17f, 1.0f), w(w), h(h), aspect(float(w)/float(h))
        , fullscreen(fullscreen), vsync(vsync), VAO(0), VBO(0), currentProgram(0)
    {
        InitRender(title, fullscreen, vsync);
        for (const auto& [name, mask] : shaders) {
            SetProgram(name, true);

            if (mask & Vertex) AttachShader(name, "external/shaders/" + name + ".vert", GL_VERTEX_SHADER);
            if (mask & Fragment) AttachShader(name, "external/shaders/" + name + ".frag", GL_FRAGMENT_SHADER);
            if (mask & Geometry) AttachShader(name, "external/shaders/" + name + ".geom", GL_GEOMETRY_SHADER);
            if (mask & TessControl) AttachShader(name, "external/shaders/" + name + ".tesc", GL_TESS_CONTROL_SHADER);
            if (mask & TessEval) AttachShader(name, "external/shaders/" + name + ".tese", GL_TESS_EVALUATION_SHADER);
            if (mask & Compute) AttachShader(name, "external/shaders/" + name + ".comp", GL_COMPUTE_SHADER);

            glLinkProgram(ShaderPrograms[name]);
            GLint isLinked = false;
            glGetProgramiv(ShaderPrograms[name], GL_LINK_STATUS, &isLinked);
            if (!isLinked) {
                char log[256];
                glGetProgramInfoLog(ShaderPrograms[name], 256, nullptr, log);
                Debug::Log("Shader program linking failed: " + std::string(log), 0);
            }
        }

        Debug::Log("Renderer initialized with custom shader programs",1);
    }

    void Render::Draw(const std::vector<VMath::Tri3>& tris) {
        if (tris.empty()) return;

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, tris.size() * sizeof(VMath::Tri3), tris.data(), GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(tris.size() * 3));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    void Render::DrawPoints(const std::vector<VMath::Vec2>& points) {
        if (points.empty()) return;

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        std::vector<VMath::Vec3> vertices;
        vertices.reserve(points.size());
        for (const auto& p : points) {
            vertices.emplace_back(p.x, p.y, 0.0f);
        }

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VMath::Vec3), vertices.data(), GL_DYNAMIC_DRAW);

        glEnable(GL_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices.size()));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
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
                if (h >= VMath::EPSILON) aspect = float(w) / float(h);
                glViewport(0, 0, w, h);
                Debug::Log("EVENT: Window resized: " + std::to_string(w) + "x" + std::to_string(h), 3);
            }
        }

        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Render::Update() {
        Uint32 currentFrameTime = SDL_GetTicks();

        deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f;
        fpsTimer += deltaTime;
        frameCount++;
        if (fpsTimer >= 5.0f) {
            Debug::Log("FPS: " + std::to_string(frameCount), 3);

            frameCount = 0;
            fpsTimer = 0.0f;
        }
        lastFrameTime = currentFrameTime;
    }

    Render::~Render() {
        Debug::Log("Cleanup Renderer", 2);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        for (auto const& [name, program] : ShaderPrograms) glDeleteProgram(program);

        if (glContext) {
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

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            glDeleteShader(shader);
            throw std::runtime_error("Shader Compilation Failed: " + std::string(infoLog));
        }

        glAttachShader(program->second, shader);
        glDeleteShader(shader);
    }

    void Render::SetProgram(GLuint program) {
        if (currentProgram != program) {
            glUseProgram(program);
            currentProgram = program;

            GLint active = 0;
            glGetIntegerv(GL_CURRENT_PROGRAM, &active);
            Debug::Log("SetProgram: requested " + std::to_string(program) + " -> GL_ACTIVE " + std::to_string(active), 3);
        }
    }

    void Render::SetProgram(const std::string& name, bool Force) {
        Debug::Log("Choosing program: " + name, 2);
        auto it = ShaderPrograms.find(name);

        if (it == ShaderPrograms.end()) {
            if (!Force) { Debug::Log("ERROR: Program not found: " + name, 1); return; }
            else { Debug::Log("Program not found: " + name + " (forcing creation)", 2); }
            GLuint program = glCreateProgram();
            it = ShaderPrograms.emplace(name, program).first;
        }

        if (currentProgram != it->second) {
            glUseProgram(it->second);
            currentProgram = it->second;
        }
    }

    void ensure_program_bound_for_uniform(GLuint currentProgram) {
        GLint active = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &active);
        if (static_cast<GLuint>(active) != currentProgram) {
            glUseProgram(currentProgram);
        }
    }

    void Render::SetUniform(const std::string& name, int x) {
        if (currentProgram == 0) {
                Debug::Log("ERROR: No program bound when setting uniform: " + name, 1);
                return;
            }
            ensure_program_bound_for_uniform(currentProgram);

            std::string key = programUniformKey(currentProgram, name);
            GLint loc;
            auto it = uniformCache.find(key);
            if (it != uniformCache.end()) loc = it->second;
            else { loc = glGetUniformLocation(currentProgram, name.c_str()); uniformCache.emplace(key, loc); }

            if (loc != -1) glUniform1i(loc, x);
            else Debug::Log("ERROR: Uniform not found: " + name, 1); }
    void Render::SetUniform(const std::string& name, float x) {
        if (currentProgram == 0) {
            Debug::Log("ERROR: No program bound when setting uniform: " + name, 1);
            return;
        }
        ensure_program_bound_for_uniform(currentProgram);

        std::string key = programUniformKey(currentProgram, name);
        GLint loc;
        auto it = uniformCache.find(key);
        if (it != uniformCache.end()) loc = it->second;
        else { loc = glGetUniformLocation(currentProgram, name.c_str()); uniformCache.emplace(key, loc); }

        if (loc != -1) glUniform1f(loc, x);
        else Debug::Log("ERROR: Uniform not found: " + name, 1); }
    void Render::SetUniform(const std::string& name, VMath::Vec2 x) {
        if (currentProgram == 0) {
            Debug::Log("ERROR: No program bound when setting uniform: " + name, 1);
            return;
        }
        ensure_program_bound_for_uniform(currentProgram);

        std::string key = programUniformKey(currentProgram, name);
        GLint loc;
        auto it = uniformCache.find(key);
        if (it != uniformCache.end()) loc = it->second;
        else { loc = glGetUniformLocation(currentProgram, name.c_str()); uniformCache.emplace(key, loc); }

        if (loc != -1) glUniform2f(loc, x.x, x.y);
        else Debug::Log("ERROR: Uniform not found: " + name, 1); }
    void Render::SetUniform(const std::string& name, VMath::Vec3 x) {
        if (currentProgram == 0) {
            Debug::Log("ERROR: No program bound when setting uniform: " + name, 1);
            return;
        }
        ensure_program_bound_for_uniform(currentProgram);

        std::string key = programUniformKey(currentProgram, name);
        GLint loc;
        auto it = uniformCache.find(key);
        if (it != uniformCache.end()) loc = it->second;
        else { loc = glGetUniformLocation(currentProgram, name.c_str()); uniformCache.emplace(key, loc); }

        if (loc != -1) glUniform3f(loc, x.x, x.y, x.z);
        else Debug::Log("ERROR: Uniform not found: " + name, 1); }
    void Render::SetUniform(const std::string& name, VMath::Vec4 x) {
        if (currentProgram == 0) {
            Debug::Log("ERROR: No program bound when setting uniform: " + name, 1);
            return;
        }
        ensure_program_bound_for_uniform(currentProgram);

        std::string key = programUniformKey(currentProgram, name);
        GLint loc;
        auto it = uniformCache.find(key);
        if (it != uniformCache.end()) loc = it->second;
        else { loc = glGetUniformLocation(currentProgram, name.c_str()); uniformCache.emplace(key, loc); }

        if (loc != -1) glUniform4f(loc, x.x, x.y, x.z, x.w);
        else Debug::Log("ERROR: Uniform not found: " + name, 1); }
    void Render::SetUniform(const std::string& name, VMath::m4 x) {
        if (currentProgram == 0) {
            Debug::Log("ERROR: No program bound when setting uniform: " + name, 1);
            return;
        }
        ensure_program_bound_for_uniform(currentProgram);

        std::string key = programUniformKey(currentProgram, name);
        GLint loc;
        auto it = uniformCache.find(key);
        if (it != uniformCache.end()) loc = it->second;
        else { loc = glGetUniformLocation(currentProgram, name.c_str()); uniformCache.emplace(key, loc); }

        if (loc != -1) glUniformMatrix4fv(loc, 1, GL_TRUE, x.data());
        else Debug::Log("ERROR: Uniform not found: " + name, 1); }

    void Render::Swap() { if (!window) return; SDL_GL_SwapWindow(window); }
    void Render::SetClearColor(const VMath::Vec4& color) { clearColor = color; }
}
