#pragma once
#include "Math.h"
#include "C:\msys64\mingw64\include\GLFW\glfw3.h"

class Camera {
public:
    bool rLocked;
    Vec3 position;
    Quaternion rotation;

    float yaw, pitch, roll;
    Vec3 front, right, up;

    float fov, sensitivity, speed;

    Camera();
    Camera(const Vec3& pos, const Quaternion& rot);

    m4 GetViewMatrix() const;
    m4 GetProjectionMatrix(float aspect, float zNear, float zFar) const;

    void UpdateVectors();
    void ProcessMouse(float xoffset, float yoffset);
    void ProcessKeyboard(GLFWwindow* window, float dt);
    void ProcessScroll(float xoffset, float yoffset);
};
