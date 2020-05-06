/*
 * File: decim.cpp
 *
 * Decimating noise generator
 *
 */

#include "userosc.h"
#include "decim.hpp"

DecimatedNoise::DecimatedNoise(State* s) : NoiseGenerator(s)
{
    buffer = 0.f;
    tick = 0;
    factor = 1;
}

void DecimatedNoise::Tick()
{
    tick = (tick + 1) % factor;
    if (tick == 0)
    {
        buffer = osc_white();
    }
}

float DecimatedNoise::GetValue()
{
    return buffer;
}

void DecimatedNoise::SetDecimationFactor(int f)
{
    if (f >= 1)
    {
        factor = f * 2;
    }
    else
    {
        factor = 1;
    }
}
