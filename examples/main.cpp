#include "../include/Engine.hpp"
#include "../external/libs/delaunator.hpp"

int main() {
    Debug d(3);
    std::printf("Running");

    Engine::Render r("Volta Engine", 1920, 1080, 1, 1);
    while (r.IsRunning()) {
        r.Poll();
        r.Draw({{ {-0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f }, { 0.0f,  0.5f, 0.0f } }});
        r.Swap();
    }
}
