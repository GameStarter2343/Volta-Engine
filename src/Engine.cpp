#include "../include/Engine.hpp"

namespace Engine {

    void Engine::Start(Settings settings, std::unordered_map<std::string, uint8_t> shaders) {
            Debug d(settings.debugLevel);
            Input::Init();
            const int renderWidth = static_cast<int>(settings.renderSize & 0xFFFF);
            const int renderHeight = static_cast<int>((settings.renderSize >> 16) & 0xFFFF);
            render = new Render(
                settings.title,
                renderWidth,
                renderHeight,
                settings.windowFlags & SDL_WINDOW_FULLSCREEN,
                settings.renderFlags != 0,
                shaders
            );
            isRunning = true;
        }


    void Engine::AttachRenderer(Render& render) {
        this->render = &render;
        isRunning = render.IsRunning();
        Debug::Log("Renderer attached to engine", 2);
    }

    void Engine::Poll() {
        if (!isRunning || !render) return;

        render->Poll();
        isRunning = render->IsRunning();
    }

    void Engine::Update() {
        if (!isRunning || !render) return;

        render->Update();
    }

    bool Engine::IsRunning() {
        if (!isRunning) return false;

        if (render) {
            isRunning = render->IsRunning();
        }

        return isRunning;
    }

    void Engine::Exit() {
        isRunning = false;
        if (render) {
            render->Exit();
        }

        Debug::Log("Engine exit requested", 1);
    }

    Render& Engine::GetRenderer() {
        if (!render) {
            throw std::runtime_error("Renderer is null!");
        }
        return *render;
    }

    float Engine::GetDeltaTime() {
        if (!render) return 0.0f;

        return render->GetDeltaTime();
    }
} // namespace Engine
