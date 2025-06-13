#ifndef LIBMATH_CONSTANTS_H
#define LIBMATH_CONSTANTS_H

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

constexpr float g_pi = static_cast<float>(M_PI);
constexpr float g_twoPi = 2.0f * static_cast<float>(M_PI);
constexpr float g_epsilon = 1e-6f;

#endif // LIBMATH_CONSTANTS_H


