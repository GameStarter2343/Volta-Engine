#include "../include/Render.hpp"
#include "../include/Debug.hpp"
#include "../external/libs/delaunator.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>
#include <vector>
#include <random>
#include <chrono>

struct Palette {
    Vec3 color1;
    Vec3 color2;
    Vec3 color3;

    float maxY;
    float midY;
    float minY;
};

const Palette Fire = {
    rgb(255, 255, 133), //Color1, the one at the top
    rgb(255, 175, 46),  //Color2, this one in the middle
    rgb(122, 46, 6),    //Color3, and this one at the bottom
    0.99f,   //maxY
    0.0f,    //midY
    -0.99f   //minY
};
const Palette Water = {
    rgb(133, 255, 255),
    rgb(52, 167, 255),
    rgb(0, 63, 141),
    0.99f,
    0.0f,
    -0.99f
};
const Palette Earth = {
    rgb(8, 113, 86),
    rgb(57, 169, 91),
    rgb(116, 241, 94),
    0.99f,
    0.0f,
    -0.99f
};

int main(int argc, char* argv[]) {
    // You can configure data here :)
    const int count = 150;
    const float speed = 1.5f;
    const float bound = 1.5f;
    const Palette& chosenPalette = Earth;

    Debug d(3);
    Debug::Log("Starting Triangles Animation", 1);
    Engine::Render r("Triangles Animation", 1920, 1080, true, true, {
        {"Triangle", {true, true, true, false, false, false}},
        {"Dots", {true, true, false, false, false, false}}
    });
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> posDist(-1.2f, 1.2f);
    std::uniform_real_distribution<float> dirDist(-1.0f, 1.0f);
    std::vector<VMath::Vec2> directions(count);
    std::vector<VMath::Vec2> positions(count + 4);

    positions[0] = VMath::Vec2(-bound, -bound);
    positions[1] = VMath::Vec2(bound, -bound);
    positions[2] = VMath::Vec2(bound, bound);
    positions[3] = VMath::Vec2(-bound, bound);

    for (int i = 4; i < count; ++i) {
        positions[i] = VMath::Vec2(posDist(rng), posDist(rng));
        directions[i] = VMath::Vec2(dirDist(rng), dirDist(rng)).Normalized();
    }


    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    int frameCounter = 0;
    std::vector<VMath::Tri3> cachedTris;

    Uint64 lastTime = SDL_GetTicks();
    Debug::Log("Entering main loop", 1);
    while (r.IsRunning()) {
        Uint64 now = SDL_GetTicks();
        float deltaTime = static_cast<float>(now - lastTime);
        lastTime = now;
        frameCounter++;

        r.Poll();

        for (int i = 4; i < count; ++i) {
            positions[i] += directions[i] * speed * deltaTime / 100000;
            if (positions[i].x > bound) positions[i].x = -bound;
            else if (positions[i].x < -bound) positions[i].x = bound;
            if (positions[i].y > bound) positions[i].y = -bound;
            else if (positions[i].y < -bound) positions[i].y = bound;
        }

        std::vector<double> points;
        points.reserve((count + 4) * 2);
        for (const auto& p : positions) {
            points.push_back(static_cast<double>(p.x));
            points.push_back(static_cast<double>(p.y));
        }

        std::vector<VMath::Tri3> newTris;
        try {
            delaunator::Delaunator d(points);
            newTris.reserve(d.triangles.size() / 3);

            for (size_t i = 0; i < d.triangles.size(); i += 3) {
                size_t i0 = d.triangles[i];
                size_t i1 = d.triangles[i+1];
                size_t i2 = d.triangles[i+2];

                if (i0 >= count+4 || i1 >= count+4 || i2 >= count+4) continue;

                VMath::Vec3 a(positions[i0], 0.0f);
                VMath::Vec3 b(positions[i1], 0.0f);
                VMath::Vec3 c(positions[i2], 0.0f);
                newTris.emplace_back(a, b, c);
            }
        } catch (...) {
            Debug::Log("Triangulation skipped", 3);
        }

        if (!newTris.empty()) {
            cachedTris = std::move(newTris);
        }

        if (!cachedTris.empty()) {
            r.SetProgram("Triangle");
            r.SetUniform("mvp", VMath::m4::Identity());
            r.SetUniform("minY", chosenPalette.minY);
            r.SetUniform("midY", chosenPalette.midY);
            r.SetUniform("maxY", chosenPalette.maxY);
            r.SetUniform("color1", chosenPalette.color1);
            r.SetUniform("color2", chosenPalette.color2);
            r.SetUniform("color3", chosenPalette.color3);
            r.Draw(cachedTris);
            r.SetProgram("Dots");
            r.SetUniform("mvp", VMath::m4::Identity());
            r.SetUniform("pointSizeMin", 4.0f);
            r.SetUniform("pointSizeMax", 12.0f);
            r.DrawPoints(positions);
        }

        r.Swap();
    }
}
