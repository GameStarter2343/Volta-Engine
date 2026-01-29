#define GLT_IMPLEMENTATION

#include "../external/glad/glad.h"
#include "../headers/RenderOGL.h"
#include "../external/glText/gltext.h"

#include "C:/msys64/mingw64/include/SDL3/SDL.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// ======================================================
// Shader (UNCHANGED)
// ======================================================
std::string Shader::LoadFileToString(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << "\n";
        return {};
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

unsigned int Shader::CompileShaderFromSource(const char* src, unsigned int type, const char* path) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success = 0;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "Shader compile error (" << path << "):\n" << infoLog << "\n";
    }
    return shader;
}

Shader::Shader(const char* shaderPath) {
    std::string vertexPath = std::string(shaderPath) + ".vert";
    std::string fragmentPath = std::string(shaderPath) + ".frag";

    unsigned int v = CompileShaderFromSource(
        LoadFileToString(vertexPath.c_str()).c_str(),
        GL_VERTEX_SHADER, vertexPath.c_str());

    unsigned int f = CompileShaderFromSource(
        LoadFileToString(fragmentPath.c_str()).c_str(),
        GL_FRAGMENT_SHADER, fragmentPath.c_str());

    ID = glCreateProgram();
    glAttachShader(ID, v);
    glAttachShader(ID, f);
    glLinkProgram(ID);

    glDeleteShader(v);
    glDeleteShader(f);
}

void Shader::Use() const { glUseProgram(ID); }
void Shader::SetMat4(const std::string& name, const m4& mat) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1) glUniformMatrix4fv(loc, 1, GL_TRUE, mat.data());
}
void Shader::SetVec3(const std::string& name, const Vec3& vec) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1) glUniform3f(loc, vec.x, vec.y, vec.z);
}
void Shader::SetFloat(const std::string& name, float v) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1) glUniform1f(loc, v);
}

// ======================================================
// Render (SDL3)
// ======================================================
Render::Render(Camera* camPtr, Debug* dbg, int width, int height, const char* shaderPath)
    : w(width), h(height), aspect(float(width) / height),
      debug(dbg), cam(camPtr) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        if (debug) debug->Log("Failed to initialize SDL3");
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    window = SDL_CreateWindow(
        "My Engine",
        w, h,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        if (debug) debug->Log("Failed to create SDL window");
        return;
    }

    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        if (debug) debug->Log("Failed to initialize GLAD");
        return;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_GL_SetSwapInterval(VSync ? 1 : 0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    gltInit();

    default3D = std::make_unique<Shader>(shaderPath);

    // Triangle
    float vertices[] = {
        0.0f,  0.5f, 0.0f, 1,0,0,
       -0.5f, -0.5f, 0.0f, 0,1,0,
        0.5f, -0.5f, 0.0f, 0,0,1
    };

    glGenVertexArrays(1, &triVAO);
    glGenBuffers(1, &triVBO);

    glBindVertexArray(triVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    InitUIRenderer();
}

Render::~Render() {
    gltTerminate();
    glDeleteVertexArrays(1, &triVAO);
    glDeleteBuffers(1, &triVBO);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Render::ShouldClose() const {
    return quitRequested;
}

void Render::Update() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT)
            quitRequested = true;

        if (e.type == SDL_EVENT_WINDOW_RESIZED) {
            w = e.window.data1;
            h = e.window.data2;
            glViewport(0, 0, w, h);
        }

        if (e.type == SDL_EVENT_MOUSE_MOTION && cam) {
            cam->ProcessMouse(
                (float)e.motion.xrel,
                (float)-e.motion.yrel);
        }

        if (e.type == SDL_EVENT_MOUSE_WHEEL && cam) {
            cam->ProcessScroll(0.0f, (float)e.wheel.y);
        }
    }

    Uint64 now = SDL_GetPerformanceCounter();
    float dt = (float)(now - lastCounter) / SDL_GetPerformanceFrequency();
    lastCounter = now;

    if (cam)
        cam->ProcessKeyboard(nullptr, dt); // keyboard now queried via SDL

    glClearColor(0.1f, 0.12f, 0.16f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::SwapPoll() {
    SDL_GL_SwapWindow(window);
}
