#pragma once

// Forward declarations to minimize include overhead in dependent files
struct SDL_Window;

// Dependencies
#include "../external/glad/glad.h"
#include <SDL3/SDL.h>

// Assuming VMath is available in your include path
#include "Volta-Math.hpp"

namespace Engine
{
    /**
     * @brief Manages OpenGL context, SDL Window, and basic rendering state.
     */
    class Render
    {
    public:
        Render();
        ~Render();

        // Disable copy/move to prevent resource duplication issues
        Render(const Render&) = delete;
        Render& operator=(const Render&) = delete;
        Render(Render&&) = delete;
        Render& operator=(Render&&) = delete;

        /**
         * @brief Initializes the SDL Window and OpenGL Context.
         * @param title Window title
         * @param width Initial window width
         * @param height Initial window height
         * @return true if successful, false otherwise
         */
        bool Initialize(const char* title, int width, int height);

        /**
         * @brief Cleans up OpenGL context and SDL resources.
         */
        void Shutdown();

        /**
         * @brief Called at the start of a frame. Clears buffers.
         */
        void BeginFrame();

        /**
         * @brief Called at the end of a frame. Swaps buffers.
         */
        void EndFrame();

        /**
         * @brief Updates the OpenGL viewport dimensions.
         * @param width New width
         * @param height New height
         */
        void SetViewport(int width, int height);

        /**
         * @brief Sets the background clear color.
         * @param color VMath::Vec4 (r, g, b, a) in range 0.0 - 1.0
         */
        void SetClearColor(const VMath::Vec4& color);

        /**
         * @brief Checks if the renderer is currently active.
         */
        bool IsRunning() const { return m_isRunning; }

        /**
         * @brief Retrieves the underlying SDL Window pointer.
         * (Useful for ImGui or other libraries needing window handles)
         */
        SDL_Window* GetWindow() const { return m_window; }

    private:
        bool m_isRunning;
        SDL_Window* m_window;
        SDL_GLContext m_glContext;
        VMath::Vec4 m_clearColor;
        int m_width;
        int m_height;

        void SetupOpenGLState();
    };
} // namespace Engine
