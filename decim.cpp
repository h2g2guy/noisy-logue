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
    // calculate decimation factor from mod value plus the envelope and/or LFO (as necessary)
    int32_t factor = state->modValue * 2;
    factor -= static_cast<int>(state->envToModPercentage * state->currentLevel) * 2;
    if (state->lfoTarget & TARGETSELECT_MOD)
    {
        factor += static_cast<int>(state->lfoLevel * 100);
    }
    factor = clipminmaxi32(1, factor, 400);

    // tick and check if we're ready for the next sample
    if (++tick >= factor)
    {
        buffer = osc_white();
        tick = 0;
    }
}

float DecimatedNoise::GetValue()
{
    return buffer;
}
