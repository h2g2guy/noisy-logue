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

struct State {
    float currentLevel;

    int32_t attackResistance;
    int32_t decayResistance;
    float sustainLevel;
    int32_t releaseResistance;
    int32_t releaseFactor;

    bool noteDown;
    bool attackPhaseComplete;
    bool releasePhaseComplete;
};
