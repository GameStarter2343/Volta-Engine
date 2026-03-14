#include "../include/Render.hpp"
#include "../include/Debug.hpp"
#include "../external/libs/delaunator.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>
#include <vector>
#include <random>
#include <cmath>

using namespace VMath;

int main(int argc, char* argv[]) {
    Debug d(3);
    Debug::Log("Starting Triangles Animation", 1);
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> posDist(-1.2f, 1.2f);
    std::uniform_real_distribution<float> dirDist(-1.0f, 1.0f);
    const int count = 300;
    const float speed = 1.5f;
    const float bound = 1.2f;
    std::vector<Vec2> positions(count);
    std::vector<Vec2> directions(count);

    for (int i = 0; i < count; ++i) {
        positions[i] = Vec2(posDist(rng), posDist(rng));
        directions[i] = Vec2(dirDist(rng), dirDist(rng)).Normalized();
    }

    Engine::Render r("Triangles Animation", 1920, 1080, true, true, "external/shaders/TriangleAnim");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    const int triangulateEveryNFrames = 1;
    int frameCounter = 0;
    std::vector<Tri3> cachedTris;
    r.SetUniform("mvp", VMath::m4::Identity());
    r.SetUniform("minY", -1.0f);
    r.SetUniform("maxY", 1.0f);
    r.SetUniform("color1", {0.38f, 0.22f, 0.69f});
    r.SetUniform("color2", {0.77f, 0.84f, 0.92f});
    Uint64 lastTime = SDL_GetTicks();

    while (r.IsRunning()) {
        Uint64 now = SDL_GetTicks();
        float deltaTime = static_cast<float>(now - lastTime);
        lastTime = now;
        frameCounter++;

        r.Poll();

        for (int i = 0; i < count; ++i) {
            positions[i] += directions[i] * speed * deltaTime / 100000;
            if (positions[i].x > bound) positions[i].x = -bound;
            else if (positions[i].x < -bound) positions[i].x = bound;
            if (positions[i].y > bound) positions[i].y = -bound;
            else if (positions[i].y < -bound) positions[i].y = bound;
        }

        if (frameCounter % triangulateEveryNFrames == 0) {
            std::vector<double> points;
            points.reserve(count * 2);
            for (const auto& p : positions) {
                points.push_back(static_cast<double>(p.x));
                points.push_back(static_cast<double>(p.y));
            }

            std::vector<Tri3> newTris;
            try {
                delaunator::Delaunator d(points);
                newTris.reserve(d.triangles.size() / 3);

                for (size_t i = 0; i < d.triangles.size(); i += 3) {
                    size_t i0 = d.triangles[i];
                    size_t i1 = d.triangles[i+1];
                    size_t i2 = d.triangles[i+2];

                    if (i0 >= count || i1 >= count || i2 >= count) continue;

                    Vec3 a(positions[i0], 0.0f);
                    Vec3 b(positions[i1], 0.0f);
                    Vec3 c(positions[i2], 0.0f);
                    newTris.emplace_back(a, b, c);
                }
            } catch (...) {
                Debug::Log("Triangulation skipped", 3);
            }

            if (!newTris.empty()) {
                cachedTris = std::move(newTris);
            }
        }

        if (!cachedTris.empty()) {
            r.Draw(cachedTris);
        }

        r.Swap();
    }
}
