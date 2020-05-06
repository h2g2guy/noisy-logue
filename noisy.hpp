#pragma once

/*
 * File: noisy.hpp
 *
 * Enveloped noise generator
 *
 */

#include "userosc.h"
#include "white.hpp"
#include "pink.hpp"
#include "red.hpp"
#include "decim.hpp"

#define TRUE 1
#define FALSE 0

#define ATTACK_MULT 480
#define DECAY_MULT 48000.f
#define RELEASE_MULT_POS 480

#define CAPACITANCE 1
#define TARGET_LEVEL_OFFSET 0.01f

#define NOISETYPE_WHITE 0
#define NOISETYPE_PINK 1
#define NOISETYPE_RED 2
#define NOISETYPE_DECIM 3
#define NOISETYPE_COUNT 4

#define TARGETSELECT_VOLUME 1
#define TARGETSELECT_MOD 2

class State
{
public:
    State() : whiteNoise(this), pinkNoise(this), redNoise(this), decimNoise(this) {}

    float currentLevel;

    int32_t attackResistance;
    int32_t decayResistance;
    float sustainLevel;
    int32_t releaseFactor;
    int32_t releaseResistance;

    bool noteDown;
    bool attackPhaseComplete;

    int32_t noiseType;
    NoiseGenerator* noiseGens[NOISETYPE_COUNT];

    WhiteNoise whiteNoise;
    PinkNoise pinkNoise;
    RedNoise redNoise;
    DecimatedNoise decimNoise;

    int32_t lfoTarget;
};

