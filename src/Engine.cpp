#include "../include/Engine.hpp"

#include <memory>

namespace Engine {
namespace {
std::unique_ptr<Debug> gDebug;
Render* gRender = nullptr;
bool gRunning = false;
}

void Start(uint8_t debugLevel) {
    if (debugLevel > 0) {
        gDebug = std::make_unique<Debug>(debugLevel);
    }

    gRunning = true;
    Debug::Log("Engine started", 1);
}

void AttachRenderer(Render& render) {
    gRender = &render;
    gRunning = render.IsRunning();
    Debug::Log("Renderer attached to engine", 2);
}

void Poll() {
    if (!gRunning || !gRender) return;

    gRender->Poll();
    gRunning = gRender->IsRunning();
}

void Update() {
    if (!gRunning || !gRender) return;

    gRender->Update();
}

bool IsRunning() {
    if (!gRunning) return false;

    if (gRender) {
        gRunning = gRender->IsRunning();
    }

    return gRunning;
}

void Exit() {
    gRunning = false;
    if (gRender) {
        gRender->Exit();
    }

    Debug::Log("Engine exit requested", 1);
}

float GetDeltaTime() {
    if (!gRender) return 0.0f;

    return gRender->GetDeltaTime();
}
} // namespace Engine
