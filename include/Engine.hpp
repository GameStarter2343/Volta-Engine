#pragma once

#include <SDL3/SDL_stdinc.h>
#include <cstdint>

#include "VMath.hpp"
#include "Render.hpp"
#include "Debug.hpp"
#include "Input.hpp"
struct Settings {
    const char* title;
    uint8_t debugLevel;
    uint32_t windowSize;
    uint16_t windowFlags;

    uint32_t renderSize;
    uint16_t renderFlags;
};
namespace Engine {
    class Engine {
    public:
        void Start(Settings settings, std::unordered_map<std::string, uint8_t> shaders);
        void AttachRenderer(Render& render);
        void Poll();
        void Update();
        bool IsRunning();
        void Exit();
        float GetDeltaTime();

        Render& GetRenderer();
    private:
        bool isRunning = false;
        float deltaTime = 0.0f;
        Render* render = nullptr;
        Window* window = nullptr;
    };
} // namespace Engine
