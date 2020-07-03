
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
    if (state->lfoTarget & LfoTargetFlags::NoiseMod)
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
