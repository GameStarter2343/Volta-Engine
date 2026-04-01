#include "../include/Window.hpp"
#include <SDL3/SDL_video.h>
Uint64 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
namespace Engine {
    Window::Window(const char* title, int width, int height) {
        w = width; h = height; aspect = (float)width / (float)height;
        window = SDL_CreateWindow(title, width, height, flags);
    }
    Window::~Window() {
        SDL_DestroyWindow(window);
    }
}
