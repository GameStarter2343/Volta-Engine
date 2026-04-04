#include "../include/Engine.hpp"
#include <random>
using namespace VMath;

Settings s{
    "Fluid Simulation",
    1,
    70780800,
    SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL,
    70780800,
    0
};
int main() {
    Engine::Engine engine;
    engine.Start(s, {
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
    while (engine.IsRunning()) {
        engine.Poll();
        engine.Update();
        Engine::Input::Update();
        if (Engine::Input::GetKey(SDL_SCANCODE_ESCAPE)) {
            engine.Exit();
        }
        engine.GetRenderer().SetProgram("Dots");
        engine.GetRenderer().SetUniform("mvp", VMath::m4::Identity());
        engine.GetRenderer().SetUniform("pointSizeMin", 3.0f);
        engine.GetRenderer().SetUniform("pointSizeMax", 6.0f);
        engine.GetRenderer().DrawPoints(positions);
        engine.GetRenderer().Swap();
    }
}
