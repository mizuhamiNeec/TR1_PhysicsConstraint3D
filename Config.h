#pragma once
#include <cstdint>
#include <numbers>

#include "Rect.h"

constexpr float deltaTime = 1.0f / 60.0f;

constexpr uint32_t kClientWidth = 1920;
constexpr uint32_t kClientHeight = 1080;

extern float gravity;
extern Rect viewport;

extern float reductionFactor;
extern bool bDrawDebug;

inline constexpr float deg2Rad = static_cast<float>(std::numbers::pi) / 180.0f;
inline constexpr float rad2Deg = 180.0f / static_cast<float>(std::numbers::pi);