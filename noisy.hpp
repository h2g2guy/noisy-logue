#pragma once

/*
 * File: waves.hpp
 *
 * Enveloped noise generator
 *
 */

#include "userosc.h"

#define TRUE 1
#define FALSE 0

#define ATTACK_MULT 480
#define DECAY_MULT 48000.f
#define RELEASE_MULT_POS 480

#define CAPACITANCE 1
#define TARGET_LEVEL_OFFSET 0.01f

#define NOISETYPE_WHITE 0
#define NOISETYPE_PINK 1
#define NOISETYPE_DECIM 2
#define NOISETYPE_COUNT 3

struct State
{
public:
    float currentLevel;

    int32_t attackResistance;
    int32_t decayResistance;
    float sustainLevel;
    int32_t releaseResistance;
    int32_t releaseFactor;

    bool noteDown;
    bool attackPhaseComplete;
    bool releasePhaseComplete;

    int32_t noiseType;
    NoiseGenerator* noiseGens[NOISETYPE_COUNT];

    WhiteNoise whiteNoise;
    PinkNoise pinkNoise;
    DecimatedNoise decimNoise;
};

