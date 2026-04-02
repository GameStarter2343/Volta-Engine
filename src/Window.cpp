#include "../include/Window.hpp"
#include "../include/Debug.hpp"
#include "../include/Input.hpp"

#include <stdexcept>
#include <string>

namespace Engine {
Window::Window(const char* title, int width, int height, bool fullscreen)
    : w(width), h(height), aspect(static_cast<float>(width) / static_cast<float>(height)), wasResized(false), window(nullptr) {
    Uint64 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    window = SDL_CreateWindow(title, width, height, flags);
    if (!window) {
        throw std::runtime_error("Failed to create window: " + std::string(SDL_GetError()));
    }

    Input::Init();
}

Window::~Window() {
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

bool Window::PollEvents(bool& isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        Input::HandleEvent(event);

        if (event.type == SDL_EVENT_QUIT) {
            isRunning = false;
            Debug::Log("EVENT: SDL Quit Event Called", 1);
            return false;
        }

        if (event.type == SDL_EVENT_WINDOW_RESIZED && event.window.windowID == SDL_GetWindowID(window)) {
            SetWidth(event.window.data1);
            SetHeight(event.window.data2);
            wasResized = true;
            Debug::Log("EVENT: Window resized: " + std::to_string(w) + "x" + std::to_string(h), 3);
        }
    }

    Input::Update();
    return true;
}

bool Window::Resize(int& outWidth, int& outHeight) {
    if (!wasResized) return false;

    outWidth = w;
    outHeight = h;
    wasResized = false;
    return true;
}

void Window::SetWidth(int width) {
    w = width;
    aspect = h != 0 ? static_cast<float>(w) / static_cast<float>(h) : 1.0f;
}

void Window::SetHeight(int height) {
    h = height;
    aspect = h != 0 ? static_cast<float>(w) / static_cast<float>(h) : 1.0f;
}
} // namespace Engine
