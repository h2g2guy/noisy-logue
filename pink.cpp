
/*
 * File: noisy.cpp
 *
 * Pink noise generator from white noise
 *
 */

#include "userosc.h"
#include "pink.hpp"

PinkNoise::PinkNoise(State* s) : NoiseGenerator(s)
{
    // initialize buffers
    for (int i = 0; i < PINK_NUM_BUFFERS; i++)
    {
        buffers[i] = osc_white();
    }
}

void PinkNoise::Tick()
{
    // next count
    tick++;

    // always tick the top-most oscillator
    buffers[0] = osc_white();

    // move through the bits; at the first point at which we are not divisible, update that sample and stop
    for (int i = 0; i < PINK_NUM_BUFFERS; i++)
    {
        if (tick % (1 << i) != 0)
        {
            buffers[i] = osc_white();
            break;
        }
    }
}

float PinkNoise::GetValue()
{
    float output = 0;
    for (int i = 1; i < PINK_NUM_BUFFERS; i++)
    {
        output += buffers[i];
    }
    output /= (float) PINK_NUM_BUFFERS / 2.f;

    // handle the filter
    if (state->modValue == 0)
    {
        // filter is not engaged; return the prefiltered value
        // still run the signal through a filter so we reduce clicks when switching to a highpass, though
        filter.Process(output, 20.f, true);
        return output;
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
    output = filter.Process(output, cutoff, state->modValue > 0);

    return output;
}

