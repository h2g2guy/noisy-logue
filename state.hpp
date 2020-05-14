#pragma once

/*
 * File: noisy.hpp
 *
 * Enveloped noise generator
 *
 */

#include "userosc.h"

// Forward declarations to avoid circular dependencies
class WhiteNoise;
class PinkNoise;
class RedNoise;
class DecimatedNoise;

struct State
{
public:

    float currentLevel;

    int32_t attackResistance;
    int32_t decayResistance;
    float sustainLevel;
    int32_t releaseFactor;
    int32_t releaseResistance;

    bool noteDown;
    bool attackPhaseComplete;

    int32_t noiseType;

    int32_t modValue;

    int lfoTarget;
};

