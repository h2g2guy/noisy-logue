
/*
 * File: red.hpp
 *
 * Red (brownian) noise generator
 *
 */

#include "userosc.h"
#include "red.hpp"

constexpr float leakMultiplier = 0.96f;
constexpr float newInputWeight = 0.2f;

RedNoise::RedNoise(State* s) : NoiseGenerator(s)
{
    buffer = 0.f;
}

void RedNoise::Tick()
{
    buffer = clip1m1f((buffer + (osc_white() * newInputWeight)) * leakMultiplier);
}

float RedNoise::GetValue()
{
    return buffer;
}
