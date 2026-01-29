#include "RenderOGL.h"
#pragma once
struct KeyCode {
    public:
    // System & Control
    static constexpr int Esc          = GLFW_KEY_ESCAPE;
    static constexpr int Tab          = 1;
    static constexpr int Caps         = 2;
    static constexpr int LeftShift    = 3;
    static constexpr int RightShift   = 4;
    static constexpr int LeftControl  = 5;
    static constexpr int RightControl = 6;
    static constexpr int LeftAlt      = 7;
    static constexpr int RightAlt     = 8;
    static constexpr int Windows      = 9;
    static constexpr int Space        = 10;
    static constexpr int Enter        = 11;
    static constexpr int Backspace    = 12;
    static constexpr int Delete       = 13;

    // Alphanumeric | Numbers
    static constexpr int N0 = 14;
    static constexpr int N1 = 15;
    static constexpr int N2 = 16;
    static constexpr int N3 = 17;
    static constexpr int N4 = 18;
    static constexpr int N5 = 19;
    static constexpr int N6 = 20;
    static constexpr int N7 = 21;
    static constexpr int N8 = 22;
    static constexpr int N9 = 23;

    // Alphanumeric - Letters
    static constexpr int A = 24; static constexpr int B = 25; static constexpr int C = 26;
    static constexpr int D = 27; static constexpr int E = 28; static constexpr int F = 29;
    static constexpr int G = 30; static constexpr int H = 31; static constexpr int I = 32;
    static constexpr int J = 33; static constexpr int K = 34; static constexpr int L = 35;
    static constexpr int M = 36; static constexpr int N = 37; static constexpr int O = 38;
    static constexpr int P = 39; static constexpr int Q = 40; static constexpr int R = 41;
    static constexpr int S = 42; static constexpr int T = 43; static constexpr int U = 44;
    static constexpr int V = 45; static constexpr int W = 46; static constexpr int X = 47;
    static constexpr int Y = 48; static constexpr int Z = 49;

    // Navigation
    static constexpr int Up       = 50;
    static constexpr int Down     = 51;
    static constexpr int Left     = 52;
    static constexpr int Right    = 53;
    static constexpr int PageUp   = 54;
    static constexpr int PageDown = 55;
    static constexpr int End      = 56;

    // Punctuation | Symbols
    static constexpr int Tilde        = 57;
    static constexpr int Minus        = 58;
    static constexpr int Equal        = 59;
    static constexpr int LeftBracket  = 60;
    static constexpr int RightBracket = 61;
    static constexpr int Backslash    = 62;
    static constexpr int Semicolon    = 63;
    static constexpr int Quota        = 64;

    // Function Keys
    static constexpr int F1  = 65;
    static constexpr int F2  = 66;
    static constexpr int F3  = 67;
    static constexpr int F4  = 68;
    static constexpr int F5  = 69;
    static constexpr int F6  = 70;
    static constexpr int F7  = 71;
    static constexpr int F8  = 72;
    static constexpr int F9  = 73;
    static constexpr int F10 = 74;
    static constexpr int F11 = 75;
    static constexpr int F12 = 76;

    static constexpr int GamepadA = 77; // South button (Cross on PlayStation)
    static constexpr int GamepadB = 78; // East button (Circle on PlayStation)
    static constexpr int GamepadX = 79; // West button (Square on PlayStation)
    static constexpr int GamepadY = 80; // North button (Triangle on PlayStation)

    // Gamepad Shoulder & Navigation
    static constexpr int GamepadL1     = 81; // Left Bumper (LB)
    static constexpr int GamepadR1     = 82; // Right Bumper (RB)
    static constexpr int GamepadL3     = 83; // Left Stick Click
    static constexpr int GamepadR3     = 84; // Right Stick Click
    static constexpr int GamepadSelect = 85; // View / Share
    static constexpr int GamepadStart  = 86; // Menu / Options

    // Gamepad D-Pad
    static constexpr int GamepadUp    = 87;
    static constexpr int GamepadDown  = 88;
    static constexpr int GamepadLeft  = 89;
    static constexpr int GamepadRight = 90;

    // Movement & Analog Axis Names (Used for float lookup)
    inline static const char* AxisHorizontal = "0";   // A/D, Left Stick X, Left/Right
    inline static const char* AxisVertical   = "1";   // W/S, Left Stick Y, Up/Down
    inline static const char* AxisMouseX     = "2";   // Mouse X, Right Stick X
    inline static const char* AxisMouseY     = "3";   // Mouse Y, Right Stick Y
    inline static const char* AxisLTrigger   = "4";   // Left Trigger (LT / L2)
    inline static const char* AxisRTrigger   = "5";   // Right Trigger (RT / R2)

};
class Input {
    bool Key[256];
    bool KeyOld[256];

    void Update() ;
    bool GetKey(int id) { return Key[id]; }
    bool GetKeyDown(int id){ return Key[id] && !KeyOld[id]; }
    bool GetKeyUp(int id) { return !Key[id] && KeyOld[id]; }
};
