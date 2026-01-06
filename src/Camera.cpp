#include "../headers/Camera.h"
#include <cmath>

Camera::Camera()
    : rLocked(false), position(0,0,0), rotation(1,0,0,0),
      yaw(-90.0f), pitch(0.0f), roll(0.0f),
      front(0.0f,0.0f,-1.0f), right(1.0f,0.0f,0.0f), up(0.0f,1.0f,0.0f),
      fov(80.0f), sensitivity(0.1f), speed(2.5f) {}

Camera::Camera(const Vec3& pos, const Quaternion& rot)
    : rLocked(false), position(pos), rotation(rot),
      yaw(-90.0f), pitch(0.0f), roll(0.0f),
      front(0.0f,0.0f,-1.0f), right(1.0f,0.0f,0.0f), up(0.0f,1.0f,0.0f),
      fov(80.0f), sensitivity(0.1f), speed(2.5f) {}

m4 Camera::GetViewMatrix() const {
    return m4::LookAt(position, position + front, up);
}

m4 Camera::GetProjectionMatrix(float aspect, float zNear, float zFar) const {
    return m4::Perspective(DtoR(fov), aspect, zNear, zFar);
}

void Camera::UpdateVectors() {
    front.x = cos(DtoR(yaw)) * cos(DtoR(pitch));
    front.y = sin(DtoR(pitch));
    front.z = sin(DtoR(yaw)) * cos(DtoR(pitch));
    front.Normalize();

    right = Vec3::Cross(front, Vec3(0,1,0)).Normalized();
    up = Vec3::Cross(right, front).Normalized();
}

void Camera::ProcessMouse(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    pitch = clamp(pitch, -89.0f, 89.0f);
    UpdateVectors();
}

void Camera::ProcessKeyboard(GLFWwindow* window, float dt) {
    float velocity = speed * dt;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) velocity *= 2.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) velocity *= 0.2f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position += right * velocity;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) position += Vec3(0, 1, 0) * velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) position -= Vec3(0, 1, 0) * velocity;
}

void Camera::ProcessScroll(float xoffset, float yoffset) {
    fov -= yoffset * 5.0f;
    fov = clamp(fov, 30.0f, 120.0f);
}
