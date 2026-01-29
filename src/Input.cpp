#include "../headers/Input.h"
#include "C:/msys64/mingw64/include/GLFW/glfw3.h"
#include <cstring>

void Input::Update()
{
    memcpy(KeyOld, Key, sizeof(Key));

    // --- System & Control ---
    Key[KeyCode::Esc]          = glfwGetKey(Render::window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    Key[KeyCode::Tab]          = glfwGetKey(Render::window, GLFW_KEY_TAB) == GLFW_PRESS;
    Key[KeyCode::Caps]         = glfwGetKey(Render::window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS;
    Key[KeyCode::LeftShift]    = glfwGetKey(Render::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    Key[KeyCode::RightShift]   = glfwGetKey(Render::window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    Key[KeyCode::LeftControl]  = glfwGetKey(Render::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    Key[KeyCode::RightControl] = glfwGetKey(Render::window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
    Key[KeyCode::LeftAlt]      = glfwGetKey(Render::window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
    Key[KeyCode::RightAlt]     = glfwGetKey(Render::window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;
    Key[KeyCode::Windows]      = glfwGetKey(Render::window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS;
    Key[KeyCode::Space]        = glfwGetKey(Render::window, GLFW_KEY_SPACE) == GLFW_PRESS;
    Key[KeyCode::Enter]        = glfwGetKey(Render::window, GLFW_KEY_ENTER) == GLFW_PRESS;
    Key[KeyCode::Backspace]    = glfwGetKey(Render::window, GLFW_KEY_BACKSPACE) == GLFW_PRESS;
    Key[KeyCode::Delete]       = glfwGetKey(Render::window, GLFW_KEY_DELETE) == GLFW_PRESS;

    // --- Numbers ---
    Key[KeyCode::N0] = glfwGetKey(Render::window, GLFW_KEY_0) == GLFW_PRESS;
    Key[KeyCode::N1] = glfwGetKey(Render::window, GLFW_KEY_1) == GLFW_PRESS;
    Key[KeyCode::N2] = glfwGetKey(Render::window, GLFW_KEY_2) == GLFW_PRESS;
    Key[KeyCode::N3] = glfwGetKey(Render::window, GLFW_KEY_3) == GLFW_PRESS;
    Key[KeyCode::N4] = glfwGetKey(Render::window, GLFW_KEY_4) == GLFW_PRESS;
    Key[KeyCode::N5] = glfwGetKey(Render::window, GLFW_KEY_5) == GLFW_PRESS;
    Key[KeyCode::N6] = glfwGetKey(Render::window, GLFW_KEY_6) == GLFW_PRESS;
    Key[KeyCode::N7] = glfwGetKey(Render::window, GLFW_KEY_7) == GLFW_PRESS;
    Key[KeyCode::N8] = glfwGetKey(Render::window, GLFW_KEY_8) == GLFW_PRESS;
    Key[KeyCode::N9] = glfwGetKey(Render::window, GLFW_KEY_9) == GLFW_PRESS;

    // --- Letters (A–Z) ---
    Key[KeyCode::A] = glfwGetKey(Render::window, GLFW_KEY_A) == GLFW_PRESS;
    Key[KeyCode::B] = glfwGetKey(Render::window, GLFW_KEY_B) == GLFW_PRESS;
    Key[KeyCode::C] = glfwGetKey(Render::window, GLFW_KEY_C) == GLFW_PRESS;
    Key[KeyCode::D] = glfwGetKey(Render::window, GLFW_KEY_D) == GLFW_PRESS;
    Key[KeyCode::E] = glfwGetKey(Render::window, GLFW_KEY_E) == GLFW_PRESS;
    Key[KeyCode::F] = glfwGetKey(Render::window, GLFW_KEY_F) == GLFW_PRESS;
    Key[KeyCode::G] = glfwGetKey(Render::window, GLFW_KEY_G) == GLFW_PRESS;
    Key[KeyCode::H] = glfwGetKey(Render::window, GLFW_KEY_H) == GLFW_PRESS;
    Key[KeyCode::I] = glfwGetKey(Render::window, GLFW_KEY_I) == GLFW_PRESS;
    Key[KeyCode::J] = glfwGetKey(Render::window, GLFW_KEY_J) == GLFW_PRESS;
    Key[KeyCode::K] = glfwGetKey(Render::window, GLFW_KEY_K) == GLFW_PRESS;
    Key[KeyCode::L] = glfwGetKey(Render::window, GLFW_KEY_L) == GLFW_PRESS;
    Key[KeyCode::M] = glfwGetKey(Render::window, GLFW_KEY_M) == GLFW_PRESS;
    Key[KeyCode::N] = glfwGetKey(Render::window, GLFW_KEY_N) == GLFW_PRESS;
    Key[KeyCode::O] = glfwGetKey(Render::window, GLFW_KEY_O) == GLFW_PRESS;
    Key[KeyCode::P] = glfwGetKey(Render::window, GLFW_KEY_P) == GLFW_PRESS;
    Key[KeyCode::Q] = glfwGetKey(Render::window, GLFW_KEY_Q) == GLFW_PRESS;
    Key[KeyCode::R] = glfwGetKey(Render::window, GLFW_KEY_R) == GLFW_PRESS;
    Key[KeyCode::S] = glfwGetKey(Render::window, GLFW_KEY_S) == GLFW_PRESS;
    Key[KeyCode::T] = glfwGetKey(Render::window, GLFW_KEY_T) == GLFW_PRESS;
    Key[KeyCode::U] = glfwGetKey(Render::window, GLFW_KEY_U) == GLFW_PRESS;
    Key[KeyCode::V] = glfwGetKey(Render::window, GLFW_KEY_V) == GLFW_PRESS;
    Key[KeyCode::W] = glfwGetKey(Render::window, GLFW_KEY_W) == GLFW_PRESS;
    Key[KeyCode::X] = glfwGetKey(Render::window, GLFW_KEY_X) == GLFW_PRESS;
    Key[KeyCode::Y] = glfwGetKey(Render::window, GLFW_KEY_Y) == GLFW_PRESS;
    Key[KeyCode::Z] = glfwGetKey(Render::window, GLFW_KEY_Z) == GLFW_PRESS;

    // --- Navigation ---
    Key[KeyCode::Up]       = glfwGetKey(Render::window, GLFW_KEY_UP) == GLFW_PRESS;
    Key[KeyCode::Down]     = glfwGetKey(Render::window, GLFW_KEY_DOWN) == GLFW_PRESS;
    Key[KeyCode::Left]     = glfwGetKey(Render::window, GLFW_KEY_LEFT) == GLFW_PRESS;
    Key[KeyCode::Right]    = glfwGetKey(Render::window, GLFW_KEY_RIGHT) == GLFW_PRESS;
    Key[KeyCode::PageUp]   = glfwGetKey(Render::window, GLFW_KEY_PAGE_UP) == GLFW_PRESS;
    Key[KeyCode::PageDown] = glfwGetKey(Render::window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS;
    Key[KeyCode::End]      = glfwGetKey(Render::window, GLFW_KEY_END) == GLFW_PRESS;

    // --- Punctuation ---
    Key[KeyCode::Tilde]        = glfwGetKey(Render::window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS;
    Key[KeyCode::Minus]        = glfwGetKey(Render::window, GLFW_KEY_MINUS) == GLFW_PRESS;
    Key[KeyCode::Equal]        = glfwGetKey(Render::window, GLFW_KEY_EQUAL) == GLFW_PRESS;
    Key[KeyCode::LeftBracket]  = glfwGetKey(Render::window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS;
    Key[KeyCode::RightBracket] = glfwGetKey(Render::window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS;
    Key[KeyCode::Backslash]    = glfwGetKey(Render::window, GLFW_KEY_BACKSLASH) == GLFW_PRESS;
    Key[KeyCode::Semicolon]    = glfwGetKey(Render::window, GLFW_KEY_SEMICOLON) == GLFW_PRESS;
    Key[KeyCode::Quota]        = glfwGetKey(Render::window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS;

    // --- Function Keys ---
    Key[KeyCode::F1]  = glfwGetKey(Render::window, GLFW_KEY_F1) == GLFW_PRESS;
    Key[KeyCode::F2]  = glfwGetKey(Render::window, GLFW_KEY_F2) == GLFW_PRESS;
    Key[KeyCode::F3]  = glfwGetKey(Render::window, GLFW_KEY_F3) == GLFW_PRESS;
    Key[KeyCode::F4]  = glfwGetKey(Render::window, GLFW_KEY_F4) == GLFW_PRESS;
    Key[KeyCode::F5]  = glfwGetKey(Render::window, GLFW_KEY_F5) == GLFW_PRESS;
    Key[KeyCode::F6]  = glfwGetKey(Render::window, GLFW_KEY_F6) == GLFW_PRESS;
    Key[KeyCode::F7]  = glfwGetKey(Render::window, GLFW_KEY_F7) == GLFW_PRESS;
    Key[KeyCode::F8]  = glfwGetKey(Render::window, GLFW_KEY_F8) == GLFW_PRESS;
    Key[KeyCode::F9]  = glfwGetKey(Render::window, GLFW_KEY_F9) == GLFW_PRESS;
    Key[KeyCode::F10] = glfwGetKey(Render::window, GLFW_KEY_F10) == GLFW_PRESS;
    Key[KeyCode::F11] = glfwGetKey(Render::window, GLFW_KEY_F11) == GLFW_PRESS;
    Key[KeyCode::F12] = glfwGetKey(Render::window, GLFW_KEY_F12) == GLFW_PRESS;
}
