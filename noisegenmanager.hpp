#pragma once

/*
 * File: noisy.hpp
 *
 * Enveloped noise generator
 *
 */

#include "state.hpp"
#include "white.hpp"
#include "pink.hpp"
#include "red.hpp"
#include "decim.hpp"

class NoiseGenManager
{
public:
    NoiseGenManager(State* s) : whiteNoise(s), pinkNoise(s), redNoise(s), decimNoise(s)
    {
        array[NOISETYPE_WHITE] = &whiteNoise;
        array[NOISETYPE_PINK] = &pinkNoise;
        array[NOISETYPE_RED] = &redNoise;
        array[NOISETYPE_DECIM] = &decimNoise;
    }

    NoiseGenerator* operator [](size_t i) { return array[i]; }

private:
    NoiseGenerator* array[NOISETYPE_COUNT];

    WhiteNoise whiteNoise;
    PinkNoise pinkNoise;
    RedNoise redNoise;
    DecimatedNoise decimNoise;
};

