#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <mpfr.h>

namespace Constants
{
    // 万有引力常数 (m^3 kg^-1 s^-2)
    const double GRAVITATIONAL_CONSTANT = 6.67430e-11;

    // 默认精度（位数）
    const mpfr_prec_t DEFAULT_PRECISION = 256;

    // 默认时间步长（秒）
    const double DEFAULT_TIME_STEP = 1.0;

    // 默认模拟时长（秒）
    const double DEFAULT_SIMULATION_TIME = 31536000.0; // 1年

    // 能量守恒容差
    const double ENERGY_TOLERANCE = 1e-10;

    // 最小距离（防止除零）
    const double MIN_DISTANCE = 1e-10;
}

#endif // CONSTANTS_H