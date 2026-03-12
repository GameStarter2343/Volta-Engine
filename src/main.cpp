#include <cstdio>
#include "../include/Debug.hpp"
#include "../include/Volta-Math.hpp"
#include "../include/Render.hpp"

int main() {
    Debug d(true);
    std::printf("Hello, World!");
    Debug::Log("Hello, World!");
    Engine::Render r("Volta Engine", 1920, 1080, 1, 1);
    while (r.IsRunning()) {
        r.BeginFrame();
        r.EndFrame();
    }
}
