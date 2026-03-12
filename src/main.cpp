#include <cstdio>
#include "../include/Volta-Math.hpp"
#include "../include/Render.hpp"

int main() {
    std::printf("Hello, World!");
    Engine::Render r("Volta Engine", 800, 600);
    while (r.IsRunning()) {
        r.BeginFrame();
        r.EndFrame();
    }
    r.Shutdown();
}
