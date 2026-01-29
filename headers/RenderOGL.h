#pragma once

#include <string>
#include <vector>
#include <memory>

#include "C:/msys64/mingw64/include/GLFW/glfw3.h"

#include "Math.h"
#include "Camera.h"
#include "Debug.h"
#include "UI.h"

/////////////////////////////////////
// Shader Class
/////////////////////////////////////
class Shader {
public:
    unsigned int ID;

    Shader(const char* shaderPath);
    void Use() const;
    void SetMat4(const std::string& name, const m4& mat) const;
    void SetVec3(const std::string& name, const Vec3& vec) const;
    void SetFloat(const std::string& name, float v) const;

private:
    std::string LoadFileToString(const char* path);
    unsigned int CompileShaderFromSource(const char* src, unsigned int type, const char* path);
};

/////////////////////////////////////
// Render Class
/////////////////////////////////////
class Render {
public:
    Render(Camera* camPtr, Debug* dbg, int width, int height, const char* shaderPath);
    ~Render();

    bool ShouldClose() const;
    void Update();
    void SwapPoll();

    void DrawTriangle();
    void Draw(const UIElement* elem);

    void ToggleBorderless();

    static GLFWwindow* window;

private:
    int w, h;
    float aspect;
    bool VSync;
    bool isBorderless = false;

    Debug* debug;

    Camera* cam;

    float lastFrame = 0.0f;
    float fpsTimer = 0.0f;
    int frameCount = 0;

    unsigned int triVAO, triVBO;
    std::unique_ptr<Shader> default3D;

    unsigned int uiVAO, uiVBO;
    void InitUIRenderer();
    void DrawQuad(Vec2 pos, Vec2 size, float rot, unsigned int textureID);
    void DrawTextUI(const std::string& text, Vec2 pos, float size);

    /////////////////////////////////////
    // Callbacks
    /////////////////////////////////////
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};
