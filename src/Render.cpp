#include "../include/Render.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include "../external/glad/glad.h"

namespace Engine
{
    Render::Render()
        : m_isRunning(false)
        , m_window(nullptr)
        , m_glContext(nullptr)
        , m_clearColor(0.1f, 0.1f, 0.1f, 1.0f) // Default dark grey
        , m_width(0)
        , m_height(0)
    {
    }

    Render::~Render()
    {
        Shutdown();
    }

    bool Render::Initialize(const char* title, int width, int height)
    {
        if (m_isRunning)
        {
            SDL_Log("Render::Initialize called but renderer is already running.");
            return false;
        }

        // 1. Initialize SDL Video
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
            return false;
        }

        // 2. Configure OpenGL Attributes (4.6 Core Profile)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        // 3. Create Window
        // SDL_WINDOW_OPENGL is required to create a GL context
        m_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);
        if (!m_window)
        {
            SDL_Log("Failed to create SDL Window: %s", SDL_GetError());
            SDL_Quit();
            return false;
        }

        // 4. Create OpenGL Context
        m_glContext = SDL_GL_CreateContext(m_window);
        if (!m_glContext)
        {
            SDL_Log("Failed to create OpenGL Context: %s", SDL_GetError());
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }

        // 5. Load OpenGL Functions with GLAD
        // We pass SDL_GL_GetProcAddress as the loader function
        int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
        if (version == 0)
        {
            SDL_Log("Failed to initialize GLAD");
            SDL_GL_DeleteContext(m_glContext);
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }

        // 6. Verify GL Version
        SDL_Log("OpenGL Version: %d.%d", GL_VERSION_MAJOR(version), GL_VERSION_MINOR(version));
        if (GL_VERSION_MAJOR(version) < 4 || (GL_VERSION_MAJOR(version) == 4 && GL_VERSION_MINOR(version) < 6))
        {
            SDL_Log("Warning: OpenGL version is less than 4.6");
        }

        // 7. Store dimensions and setup state
        m_width = width;
        m_height = height;
        SetupOpenGLState();

        m_isRunning = true;
        return true;
    }

    void Render::Shutdown()
    {
        if (!m_isRunning) return;

        if (m_glContext)
        {
            SDL_GL_DeleteContext(m_glContext);
            m_glContext = nullptr;
        }

        if (m_window)
        {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }

        SDL_Quit();
        m_isRunning = false;
    }

    void Render::BeginFrame()
    {
        if (!m_isRunning) return;

        glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void Render::EndFrame()
    {
        if (!m_isRunning) return;

        SDL_GL_SwapWindow(m_window);
    }

    void Render::SetViewport(int width, int height)
    {
        m_width = width;
        m_height = height;
        glViewport(0, 0, width, height);
    }

    void Render::SetClearColor(const VMath::Vec4& color)
    {
        m_clearColor = color;
    }

    void Render::SetupOpenGLState()
    {
        // Set initial viewport
        glViewport(0, 0, m_width, m_height);

        // Enable common features
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        // Enable Blending (standard alpha blending)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set clear color initially
        glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
    }

} // namespace Engine
