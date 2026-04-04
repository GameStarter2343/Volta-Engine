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
    int PointCount = 5000;
    Vec2 Bounds = {1.5f, 1.5f};
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> posDistX(-Bounds.x/2 + 0.1, Bounds.x/2 - 0.1);
    std::uniform_real_distribution<float> posDistY(-Bounds.y/2 + 0.1, Bounds.y/2 - 0.1);
    float gravity = 0.003f;
    float speed = 1.0f;
    std::vector<VMath::Vec2> positions(PointCount);
    std::vector<VMath::Vec2> directions(PointCount);

    for (int i = 0; i < PointCount; ++i) {
        positions[i] = VMath::Vec2(posDistX(rng), posDistY(rng));
        directions[i] = VMath::Vec2();
    }
    float halfX = Bounds.x / 2.0f;
    float halfY = Bounds.y / 2.0f;
    while (engine.IsRunning()) {
        engine.Poll();
        engine.Update();
        Engine::Input::Update();
        if (Engine::Input::GetKey(SDL_SCANCODE_ESCAPE)) {
            engine.Exit();
        }
        if (Engine::Input::GetKey(SDL_SCANCODE_SPACE)) {
            speed = -speed;
        }
        if (Engine::Input::GetKeyDown(SDL_SCANCODE_LEFT)) {
            speed /= 0.9f;
        }
        if (Engine::Input::GetKeyDown(SDL_SCANCODE_RIGHT)) {
            speed *= 0.9f;
        }
        for (int i = 0; i < PointCount; ++i) {
            // X axis
            if (positions[i].x < -halfX) {
                positions[i].x = -halfX;
                if (directions[i].x < 0.0f)
                    directions[i].x = -directions[i].x;
            }
            else if (positions[i].x > halfX) {
                positions[i].x = halfX;
                if (directions[i].x > 0.0f)
                    directions[i].x = -directions[i].x;
            }

            // Y axis
            if (positions[i].y < -halfY) {
                positions[i].y = -halfY;
                if (directions[i].y < 0.0f)
                    directions[i].y = -directions[i].y;
            }
            else if (positions[i].y > halfY) {
                positions[i].y = halfY;
                if (directions[i].y > 0.0f)
                    directions[i].y = -directions[i].y;
            }

            // Physics
            if (speed < 0) continue;
            directions[i] += VMath::Vec2(0.0f, -gravity * VPhysics::g * engine.GetDeltaTime() * speed);
            positions[i] += directions[i] * engine.GetDeltaTime() * 10 * speed;
        }
        engine.GetRenderer().SetProgram("Dots");
        engine.GetRenderer().SetUniform("color", VMath::rgba(80, 193, 237));
        engine.GetRenderer().SetUniform("mvp", VMath::m4::Identity());
        engine.GetRenderer().SetUniform("pointSizeMin", 10.0f);
        engine.GetRenderer().SetUniform("pointSizeMax", 10.0f);
        engine.GetRenderer().DrawPoints(positions);
        engine.GetRenderer().Swap();
    }
}
