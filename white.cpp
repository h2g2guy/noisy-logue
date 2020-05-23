
/*
 * File: white.cpp
 *
 * White noise generator; wrapper of osc_white
 *
 */


#include "userosc.h"
#include "white.hpp"

WhiteNoise::WhiteNoise(State* s) : NoiseGenerator(s)
{
    Tick();
}

void WhiteNoise::Tick()
{
    buffer = osc_white();
}

float WhiteNoise::GetValue()
{
    if (state->modValue == 0)
    {
        // filter is not engaged; return the prefiltered value
        // still run the signal through a filter so we reduce clicks when switching to a highpass, though
        filter.Process(buffer, 20.f, true);
        return buffer;
    }

    // filter is engaged
    float modulation = 0.f;

    // account for LFO modulation
    if (state->lfoTarget & TARGETSELECT_MOD)
    {
        modulation += state->lfoLevel;
    }

    // account for envelope modulation
    if (state->envToModPercentage != 0)
    {
        modulation += ((float)state->envToModPercentage / 100.f) * state->currentLevel;
    }

    float cutoff = ModValueToFrequency(state->modValue, modulation);
    float output = filter.Process(buffer, cutoff, state->modValue > 0);
    return output;
}
