#include "../include/Engine.hpp"
#include <SDL3/SDL_scancode.h>
#include <random>
using namespace VMath;

Settings s{
    "Fluid Simulation",
    1,
    1080 << 16 | 1920,
    SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL,
    1080 << 16 | 1920,
    0
};
int main() {
    Engine::Engine engine;
    engine.Start(s, {
        {"Dots", Vertex | Fragment},
        {"basic", Vertex | Fragment},
    });

    int PointCount = 500;
    Vec2 Bounds = {1.5f, 1.5f};
    float gravity = 0.003f;
    float speed = 1.0f;
    float halfX = Bounds.x / 2.0f;
    float halfY = Bounds.y / 2.0f;

    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> posDistX(-halfX, halfX);
    std::uniform_real_distribution<float> posDistY(-halfY, halfY);
    std::vector<VMath::Vec2> positions(PointCount);
    std::vector<VMath::Vec2> directions(PointCount);

    for (int i = 0; i < PointCount; ++i) {
        positions[i] = VMath::Vec2(posDistX(rng), posDistY(rng));
        directions[i] = VMath::Vec2();
    }

    while (engine.IsRunning()) {
        engine.Poll();
        engine.Update();
        if (Engine::Input::GetKey(SDL_SCANCODE_ESCAPE)) {
            engine.Exit();
        }
        if (Engine::Input::GetKey(SDL_SCANCODE_SPACE)) {
            speed = -speed;
        }
        if (Engine::Input::GetKey(SDL_SCANCODE_UP)) {
            speed /= 0.995f;
        }
        if (Engine::Input::GetKey(SDL_SCANCODE_DOWN)) {
            speed *= 0.995f;
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
        engine.GetRenderer().SetUniform("mvp", VMath::m4::Ortho(-1, 1, -1, 1, -1.0f, 1.0f));
        engine.GetRenderer().SetUniform("pointSizeMin", 10.0f);
        engine.GetRenderer().SetUniform("pointSizeMax", 10.0f);
        engine.GetRenderer().DrawPoints(positions);
        engine.GetRenderer().Swap();
    }
}
