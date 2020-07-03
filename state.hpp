#pragma once

/*
 * File: noisy.hpp
 *
 * Enveloped noise generator
 *
 */

#include "userosc.h"

class LfoTargetFlags
{
public:
    typedef enum : int32_t
    {
        None = 0,
        Volume = 1,
        NoiseMod = 2
    } enum_type;

    LfoTargetFlags(enum_type targets = None) : value(targets) { }

    operator enum_type() const { return value; }

    bool operator&(int32_t operand) { return static_cast<int32_t>(value) & operand; }

private:
    enum_type value;
};

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

    int32_t lfoTarget; // bitfield of LfoTargetFlags
    int32_t envToModPercentage;
};

