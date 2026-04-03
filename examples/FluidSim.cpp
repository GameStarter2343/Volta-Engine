#include "../include/Engine.hpp"
using namespace VMath;
int main() {
    Engine::Start(3);
    Engine::Render r("Triangles Animation", 1920, 1080, 1, 1, {
        {"Dots", Vertex | Fragment},
    });
    int PointCount = 100;
    Vec2 Bounds = {1.5f, 1.5f};
    while (r.IsRunning()) {
        r.Poll();
        r.Update();
        Engine::Input::Update();
        if (Engine::Input::GetKey(SDL_SCANCODE_ESCAPE)) {
            r.Exit();
        }

        r.Swap();
    }
}
