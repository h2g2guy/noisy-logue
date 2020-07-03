#pragma once

/*
 * File: twopole.hpp
 *
 * Simple, two pole filter design
 *
 */

#include "float_math.h"

class Filter
{
public:
    Filter();
    float Process(float sample, float cutoff, bool isHighPass);

private:
    float a1, a2, b0, b1, b2; // normalized by dividing by a0
    const float q = M_1_SQRT2; // always use 1/sqrt(2) for a predictable curve
    float x1, x2; // previous two samples
    float y1, y2; // previous two results

    void calculateConstants(float cutoff, bool isHighPass);
};

float ModValueToFrequency(int modValue, float modulation = 0.f);
