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
}

void DecimatedNoise::Tick()
{
    // calculate decimation factor from mod value
    int factor;
    if (state->modValue >= 1)
    {
        factor = state->modValue * 2;
    }
    else
    {
        factor = 1;
    }

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
