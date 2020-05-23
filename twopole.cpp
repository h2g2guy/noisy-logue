/*
 * File: twopole.cpp
 *
 * Simple, two pole filter design
 *
 */

#include "float_math.h"
#include "osc_api.h"
#include "twopole.hpp"

Filter::Filter() {}

float Filter::Process(float x0, float cutoff, bool isHighPass)
{
    calculateConstants(cutoff, isHighPass);

    // calculate result
    float y0 = b0 * x0 + b1 * x1 + b2 * x2 - (a1 * y1 + a2 * y2);

    // cascade results
    x2 = x1;
    x1 = x0;
    y2 = y1;
    y1 = y0;

    return y0;
}

void Filter::calculateConstants(float cutoff, bool isHighPass)
{
    float omega = 2 * M_PI * cutoff / k_samplerate;
    float cosOmega = fastcosf(omega);
    float sinOmega = fastsinf(omega);

    float alpha = sinOmega / (2 * q);

    if (isHighPass)
    {
        a1 = -2.f * cosOmega;
        a2 = 1.f - alpha;

        b0 = (1.f + cosOmega) / 2.f;
        b1 = -1.f * (1.f + cosOmega);
        b2 = b0;
    }
    else // low pass
    {
        a1 = -2.f * cosOmega;
        a2 = 1.f - alpha;

        b0 = (1.f - cosOmega) / 2.f;
        b1 = 1.f - cosOmega;
        b2 = b0;
    }

    // normalize
    float a0 = 1 + alpha;
    a1 = a1 / a0;
    a2 = a2 / a0;
    b0 = b0 / a0;
    b1 = b1 / a0;
    b2 = b2 / a0;
}

float ModValueToFrequency(int modValue, float modulation)
{
    float x = (float)modValue + (modulation * 100.f);
    if (modValue < 0)
    {
        x += 100.f;
    }

    return clipminmaxf(20.f, 20.f * fasterpowf(1000.f, x / 100.f), 20000.f);
}
