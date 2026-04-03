#include "../include/Engine.hpp"
#include <random>
using namespace VMath;
int main() {
    Engine::Start(3);
    Engine::Render r("Triangles Animation", 1920, 1080, 1, 1, {
        {"Dots", Vertex | Fragment},
    });
    int PointCount = 100;
    Vec2 Bounds = {1.5f, 1.5f};
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> posDist(-1.0f, 1.0f);
    std::vector<VMath::Vec2> positions(PointCount + 4);

    for (int i = 4; i < PointCount + 4; ++i) {
        positions[i] = VMath::Vec2(posDist(rng), posDist(rng));
    }
    while (r.IsRunning()) {
        r.Poll();
        r.Update();
        Engine::Input::Update();
        if (Engine::Input::GetKey(SDL_SCANCODE_ESCAPE)) {
            r.Exit();
        }
        r.SetProgram("Dots");
        r.SetUniform("mvp", VMath::m4::Identity());
        r.SetUniform("pointSizeMin", 3.0f);
        r.SetUniform("pointSizeMax", 6.0f);
        r.DrawPoints(positions);
        r.Swap();
    }
}
