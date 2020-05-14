
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
    bool isLfoEngaged = (state->lfoTarget & TARGETSELECT_MOD);
    float cutoff = ModValueToFrequency(state->modValue, (isLfoEngaged ? state->lfoLevel : 0.f));
    float output = filter.Process(buffer, cutoff, state->modValue > 0);
    return output;
}
