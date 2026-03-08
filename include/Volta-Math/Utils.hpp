#include <cmath>
#pragma once

inline constexpr float PI      = 3.1415926535f;
inline constexpr float TWO_PI  = 6.2831853071f;
inline constexpr float HALF_PI = 1.5707963267f;
inline constexpr float INV_PI  = 0.3183098861f;
inline constexpr float DEG2RAD = PI / 180.0f;
inline constexpr float RAD2DEG = 180.0f / PI;
inline constexpr float DtoR(float D) { return D * DEG2RAD; }
inline constexpr float RtoD(float R) { return R * RAD2DEG; }

inline float clamp(float value, float min, float max) { return fmax(min, fmin(value, max)); }
inline bool IsZero(float a) { return std::abs(a) < 0.000001; }
inline bool IsEqual(float a, float b) { return std::abs(a - b) < 0.000001; }
