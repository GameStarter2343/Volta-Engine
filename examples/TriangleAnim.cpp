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
    Debug::Log("Starting Delaunay Animation", 1);
    std::mt19937 rng(42); // Fixed seed for reproducibility
    std::uniform_real_distribution<float> posDist(-1.2f, 1.2f);
    std::uniform_real_distribution<float> dirDist(-1.0f, 1.0f);

    // Create 150 points and directions
    std::vector<Vec2> positions(300);
    std::vector<Vec2> directions(300);

    for (int i = 0; i < 300; ++i) {
        positions[i] = Vec2(posDist(rng), posDist(rng));
        directions[i] = Vec2(dirDist(rng), dirDist(rng)).Normalized();
    }

    Engine::Render render("Delaunay Animation", 1920, 1080, 1, 1);

    // Visual setup
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);  // Keep this for correct winding
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Triangulation throttling
    const int triangulateEveryNFrames = 1;
    int frameCounter = 0;
    std::vector<Tri3> cachedTris;  // Reuse triangles when skipping triangulation

    Uint64 lastTime = SDL_GetTicks();
    const float speed = 3.0f;  // You can increase speed now!
    const float bound = 1.2f;

    while (render.IsRunning()) {
        Uint64 now = SDL_GetTicks();
        float deltaTime = static_cast<float>(now - lastTime);
        lastTime = now;
        frameCounter++;

        render.Poll();

        // === ALWAYS UPDATE POSITIONS (smooth motion) ===
        for (int i = 0; i < 300; ++i) {
            positions[i] += directions[i] * speed * deltaTime / 100000;
            if (positions[i].x > bound) positions[i].x = -bound;
            else if (positions[i].x < -bound) positions[i].x = bound;
            if (positions[i].y > bound) positions[i].y = -bound;
            else if (positions[i].y < -bound) positions[i].y = bound;
        }

        // === ONLY TRIANGULATE EVERY N FRAMES ===
        if (frameCounter % triangulateEveryNFrames == 0) {
            std::vector<double> points;
            points.reserve(600);
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

                    if (i0 >= 300 || i1 >= 300 || i2 >= 300) continue;

                    Vec3 a(positions[i0], 0.0f);
                    Vec3 b(positions[i1], 0.0f);
                    Vec3 c(positions[i2], 0.0f);
                    newTris.emplace_back(a, b, c);
                }
            } catch (...) {
                Debug::Log("Triangulation skipped", 3);
            }

            // Atomically update cache (avoid tearing)
            if (!newTris.empty()) {
                cachedTris = std::move(newTris);
            }
        }

        // === RENDER CACHED TRIANGLES ===
        if (!cachedTris.empty()) {
            render.Draw(cachedTris);
        }

        render.Swap();
    }
}
