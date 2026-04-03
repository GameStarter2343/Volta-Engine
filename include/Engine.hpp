#pragma once

#include <cstdint>

#include "VMath.hpp"
#include "Render.hpp"
#include "Debug.hpp"
#include "Input.hpp"

namespace Engine {
void Start(uint8_t debugLevel = 0);
void AttachRenderer(Render& render);
void Poll();
void Update();
bool IsRunning();
void Exit();
float GetDeltaTime();
} // namespace Engine
