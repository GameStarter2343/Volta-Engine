#define GLT_IMPLEMENTATION

#include "../external/glad/glad.h"
#include "../headers/Render.h"
#include "../external/glText/gltext.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "C:\msys64\mingw64\include\GLFW\glfw3.h"

/////////////////////////////////////
// Shader
/////////////////////////////////////
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

Shader::Shader(const char* shaderPath){
    std::string vertexPath = std::string(shaderPath) + ".vert";
    std::string fragmentPath = std::string(shaderPath) + ".frag";

    std::string vsrc = LoadFileToString(vertexPath.c_str());
    std::string fsrc = LoadFileToString(fragmentPath.c_str());

    unsigned int v = CompileShaderFromSource(vsrc.c_str(), GL_VERTEX_SHADER, vertexPath.c_str());
    unsigned int f = CompileShaderFromSource(fsrc.c_str(), GL_FRAGMENT_SHADER, fragmentPath.c_str());

    ID = glCreateProgram();
    glAttachShader(ID, v);
    glAttachShader(ID, f);
    glLinkProgram(ID);

    int success = 0;
    char infoLog[1024];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 1024, nullptr, infoLog);
        std::cerr << "Shader link error:\n" << infoLog << "\n";
    }

    glDeleteShader(v);
    glDeleteShader(f);
}

void Shader::Use() const { glUseProgram(ID); }
void Shader::SetMat4(const std::string& name, const m4& mat) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1) glUniformMatrix4fv(loc, 1, GL_TRUE, mat.data()); }
void Shader::SetVec3(const std::string& name, const Vec3& vec) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1) glUniform3f(loc, vec.x, vec.y, vec.z); }
void Shader::SetFloat(const std::string& name, float v) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1) glUniform1f(loc, v); }

/////////////////////////////////////
// Render
/////////////////////////////////////

Render::Render(Camera* camPtr, Debug* dbg, int width, int height, const char* shaderPath)
    : w(width), h(height), aspect(float(width)/height), VSync(false),
      window(nullptr), debug(dbg), cam(camPtr)
{
    if (!glfwInit()) {
        if(debug) debug->Log("Failed to initialize GLFW");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(w, h, "My Engine", nullptr, nullptr);
    if (!window) {
        if(debug) debug->Log("Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        if(debug) debug->Log("Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
        return;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(VSync ? 1 : 0);
    glfwSetWindowUserPointer(window, cam);
    ToggleBorderless();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    gltInit();
    if(debug) debug->Log("Render system initialized");

    // Setup shaders and triangle
    default3D = std::make_unique<Shader>(shaderPath);

    float vertices[] = {
        // positions       // colors
        0.0f,  0.5f, 0.0f, 1.0f,0.0f,0.0f,
       -0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,
        0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f
    };
    glGenVertexArrays(1, &triVAO);
    glGenBuffers(1, &triVBO);
    glBindVertexArray(triVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glBindVertexArray(0);

    InitUIRenderer();
}

Render::~Render() {
    gltTerminate();
    if(window) glfwDestroyWindow(window);
    glDeleteVertexArrays(1, &triVAO);
    glDeleteBuffers(1, &triVBO);
}

bool Render::ShouldClose() const {
    return window && (glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);
}

void Render::Update() {
    if(!window) return;

    float currentFrame = glfwGetTime();
    float dt = currentFrame - lastFrame;
    fpsTimer += dt;
    lastFrame = currentFrame;
    frameCount++;

    if(fpsTimer >= 1.0f) {
        if(debug) debug->Log("FPS: " + std::to_string(frameCount));
        frameCount = 0;
        fpsTimer -= 1.0f;
    }

    if(cam) cam->ProcessKeyboard(window, dt);

    glClearColor(0.1f, 0.12f, 0.16f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::SwapPoll() {
    if(window) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

/////////////////////////////////////
// Triangle Drawing
/////////////////////////////////////
void Render::DrawTriangle() {
    default3D->Use();
    glBindVertexArray(triVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

/////////////////////////////////////
// UI Renderer
/////////////////////////////////////
void Render::InitUIRenderer() {
    float quadVertices[] = {
        // pos      // uv
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &uiVAO);
    glGenBuffers(1, &uiVBO);

    glBindVertexArray(uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

void Render::DrawQuad(Vec2 pos, Vec2 size, float rot, unsigned int textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(uiVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Render::DrawTextUI(const std::string& text, Vec2 pos, float size) {
    static GLTtext* t = nullptr;
    if(!t) t = gltCreateText();
    gltSetText(t, text.c_str());
    gltColor(1,1,1,1);
    gltDrawText2D(t, pos.x, pos.y, size);
}

/////////////////////////////////////
// Draw UI element tree
/////////////////////////////////////
void Render::Draw(const UIElement* elem) {
    if(!elem) return;

    if(const UIPanel* panel = dynamic_cast<const UIPanel*>(elem)) {
        if(!panel->backgroundTexture.empty())
            DrawQuad(panel->position, panel->size, panel->rotation, std::stoi(panel->backgroundTexture));
    } else if(const UIImage* image = dynamic_cast<const UIImage*>(elem)) {
        DrawQuad(image->position, image->scale, image->rotation, std::stoi(image->texturePath));
    } else if(const UIText* text = dynamic_cast<const UIText*>(elem)) {
        DrawTextUI(text->text, text->position, text->fontSize);
    }

    for(const auto& child : elem->children)
        Draw(child.get());
}

/////////////////////////////////////
// Window Callbacks
/////////////////////////////////////
void Render::framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0,0,width,height);
}

void Render::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = 960.0f, lastY = 540.0f;
    static bool firstMouse = true;

    Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    if(!cam) return;

    if(firstMouse) { lastX = (float)xpos; lastY = (float)ypos; firstMouse = false; }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;
    lastX = (float)xpos;
    lastY = (float)ypos;

    cam->ProcessMouse(xoffset, yoffset);
}

void Render::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    if(cam) cam->ProcessScroll(xoffset, yoffset);
}

/////////////////////////////////////
// Borderless
/////////////////////////////////////
void Render::ToggleBorderless() {
    isBorderless = !isBorderless;
    if(!window) return;

    if(isBorderless) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
        glfwSetWindowMonitor(window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
    }

    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0,0,w,h);
}
