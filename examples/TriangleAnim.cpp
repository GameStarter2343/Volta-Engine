#include "../include/Engine.hpp"
#include "../external/libs/delaunator.hpp"
#include <vector>
#include <random>
#include <chrono>
using Stage = Engine::Render;
struct Palette {
    VMath::Vec3 color1;
    VMath::Vec3 color2;
    VMath::Vec3 color3;

    float maxY;
    float midY;
    float minY;
};

const Palette Fire = {
    VMath::rgb(255, 255, 163), //Color1, the one at the top
    VMath::rgb(255, 175, 46),  //Color2, this one in the middle
    VMath::rgb(122, 46, 6),    //Color3, and this one at the bottom
    0.99f,   //maxY
    0.0f,    //midY
    -0.99f   //minY
};
const Palette Water = {
    VMath::rgb(123, 235, 255),
    VMath::rgb(32, 147, 255),
    VMath::rgb(0, 43, 121),
    0.99f,
    0.0f,
    -1.01f
};
const Palette Earth = {
    VMath::rgb(8, 113, 86),
    VMath::rgb(57, 169, 91),
    VMath::rgb(116, 241, 94),
    0.99f,
    0.0f,
    -0.99f
};
const Palette Air = {
    VMath::rgb(52, 52, 52),
    VMath::rgb(121, 121, 121),
    VMath::rgb(225, 225, 225),
    0.99f,
    0.0f,
    -0.99f
};
const Palette Space = {
    VMath::rgb(96, 53, 172),
    VMath::rgb(143, 145, 220),
    VMath::rgb(200, 216, 235),
    0.99f,
    0.0f,
    -0.99f
};
Settings s{
    "Triangle Animation",
    1,
    70780800,
    SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL,
    70780800,
    0
};
int main() {
    // You can configure data here :)
    const int count = 300; // Vertices count
    const float speed = 1.0f; // Vertices move
    const float bound = 1.2f; // Box size, make it more than 1 to prevent artifacts
    const Palette& chosenPalette = Water;
    Engine::Engine engine;
    engine.Start(s, {
        {"Triangle", Vertex | Fragment | Geometry},
        {"Dots", Vertex | Fragment}
    });

    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> posDist(-bound, bound);
    std::uniform_real_distribution<float> dirDist(0.0f, 1.0f);
    std::vector<VMath::Vec2> directions(count);
    std::vector<VMath::Vec2> positions(count + 4); // Extra 4 points for the box corners to prevent artifacts

    positions[0] = VMath::Vec2(-bound, -bound);
    positions[1] = VMath::Vec2(bound, -bound);
    positions[2] = VMath::Vec2(bound, bound);
    positions[3] = VMath::Vec2(-bound, bound);

    for (int i = 4; i < count + 4; ++i) {
        positions[i] = VMath::Vec2(posDist(rng), posDist(rng));
        directions[i - 4] = VMath::Vec2(dirDist(rng)*2 - 1, dirDist(rng)*2 - 1).Normalized();
    }


    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    int frameCounter = 0;
    std::vector<VMath::Tri3> cachedTris;

    Debug::Log("Entering main loop", 1);
    while (engine.IsRunning()) {
        engine.Update();
        engine.Poll();
        if (Engine::Input::GetKey(SDL_SCANCODE_ESCAPE)) engine.Exit();

        for (int i = 4; i < count; ++i) {
            positions[i] += directions[i - 4] / 100 * speed * engine.GetDeltaTime();
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
            engine.GetRenderer().SetProgram("Triangle");
            engine.GetRenderer().SetUniform("mvp", VMath::m4::Identity());
            engine.GetRenderer().SetUniform("minY", chosenPalette.minY);
            engine.GetRenderer().SetUniform("midY", chosenPalette.midY);
            engine.GetRenderer().SetUniform("maxY", chosenPalette.maxY);
            engine.GetRenderer().SetUniform("color1", chosenPalette.color1);
            engine.GetRenderer().SetUniform("color2", chosenPalette.color2);
            engine.GetRenderer().SetUniform("color3", chosenPalette.color3);
            engine.GetRenderer().Draw(cachedTris);
            engine.GetRenderer().SetProgram("Dots");
            engine.GetRenderer().SetUniform("mvp", VMath::m4::Identity());
            engine.GetRenderer().SetUniform("pointSizeMin", 3.0f);
            engine.GetRenderer().SetUniform("pointSizeMax", 6.0f);
            engine.GetRenderer().DrawPoints(positions);
        }

        engine.GetRenderer().Swap();
    }

    engine.Exit();
}
