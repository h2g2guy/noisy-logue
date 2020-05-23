#pragma once

/*
 * File: noisy.hpp
 *
 * Enveloped noise generator
 *
 */

#include "userosc.h"

#define TARGETSELECT_VOLUME 1
#define TARGETSELECT_MOD 2

struct State
{
public:

    float currentLevel;
    float lfoLevel;

    int32_t attackResistance;
    int32_t decayResistance;
    float sustainLevel;
    int32_t releaseFactor;
    int32_t releaseResistance;

    bool noteDown;
    bool attackPhaseComplete;

    int32_t noiseType;

    int32_t modValue;

    int32_t lfoTarget;
    int32_t envToModPercentage;
};

