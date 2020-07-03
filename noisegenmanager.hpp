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

enum class NoiseType : int
{
    White = 0,
    Pink = 1,
    Red = 2,
    Decim = 3,
    Count = 4
};

class NoiseGenManager
{
public:
    NoiseGenManager(State* s) : whiteNoise(s), pinkNoise(s), redNoise(s), decimNoise(s)
    {
        array[static_cast<int>(NoiseType::White)] = &whiteNoise;
        array[static_cast<int>(NoiseType::Pink)] = &pinkNoise;
        array[static_cast<int>(NoiseType::Red)] = &redNoise;
        array[static_cast<int>(NoiseType::Decim)] = &decimNoise;
    }

    NoiseGenerator* operator [](size_t i) { return array[i]; }

private:
    NoiseGenerator* array[static_cast<int>(NoiseType::Count)];

    WhiteNoise whiteNoise;
    PinkNoise pinkNoise;
    RedNoise redNoise;
    DecimatedNoise decimNoise;
};

