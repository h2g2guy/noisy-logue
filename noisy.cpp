
/*
 * File: noisy.cpp
 *
 * Enveloped noise generator
 *
 */

#include "noisy.hpp"

static State s = {};

void calculateReleaseResistance()
{
    // Release resistance is calculated off of a baseline of decay resistance.
    // If release factor is greater than zero, add its value to decay resistance.
    // If it's less than zero, add 100 and use that as a percentage of decay resistance.
    if (s.releaseFactor >= 0)
    {
        s.releaseResistance = s.releaseFactor * RELEASE_MULT_POS + s.decayResistance;
    }
    else
    {
        float multiplier = (float)(s.releaseFactor + 100.f) / 100.f;
        s.releaseResistance = multiplier * s.decayResistance;
    }
    s.releaseResistance++; // we can't be having zero
}

void OSC_INIT(uint32_t platform, uint32_t api)
{
    (void)platform;
    (void)api;
    s.currentLevel = 0.f;
    s.attackResistance = 1;
    s.decayResistance = 1;
    s.releaseFactor = 0;
    s.sustainLevel = 0.f;
    s.noteDown = false;
    s.attackPhaseComplete = false;
    s.releasePhaseComplete = true;

    calculateReleaseResistance();
}

bool targetLevelReached(float targetLevel, float currentLevel)
{
    float offsetTarget;

    if (targetLevel == currentLevel)
    {
        return true;
    }
    else if (targetLevel > currentLevel)
    {
        offsetTarget = targetLevel - TARGET_LEVEL_OFFSET;
        return currentLevel > offsetTarget;
    }
    else
    {
        offsetTarget = targetLevel + TARGET_LEVEL_OFFSET;
        return currentLevel < offsetTarget;
    }
}

float getChangeInLevel(float targetLevel, float currentLevel, float resistance)
{
    return (targetLevel / resistance) - (currentLevel / (resistance * CAPACITANCE));
}

void getNewLevel()
{
    if (s.noteDown)
    {
        if (!(s.attackPhaseComplete))
        {
            // attack phase
            s.currentLevel = clip1f(s.currentLevel + getChangeInLevel(1.f, s.currentLevel, s.attackResistance));
            if (targetLevelReached(1.f, s.currentLevel))
            {
                s.attackPhaseComplete = true;
            }
        }
        else
        {
            // decay/sustain phase
            s.currentLevel = clipminf(s.sustainLevel, s.currentLevel +
                    getChangeInLevel(s.sustainLevel, s.currentLevel, s.decayResistance));
        }
    }
    else
    {
        if (!(s.releasePhaseComplete))
        {
            // release phase
            s.currentLevel = clip0f(s.currentLevel + getChangeInLevel(0.f, s.currentLevel, s.releaseResistance));
            if (targetLevelReached(0.f, s.currentLevel))
            {
                s.releasePhaseComplete = true;
            }
        }
        else
        {
            // "off" phase
            s.currentLevel = 0.f;
        }
    }
}

void OSC_CYCLE(const user_osc_param_t * const params,
        int32_t *yn,
        const uint32_t frames)
{
    (void)params;
    float output = 0.f;

    for (uint32_t i = 0; i < frames; i++)
    {
        getNewLevel();
        output = osc_white();
        output *= s.currentLevel;
        yn[i] = f32_to_q31(output);
    }
}

void OSC_NOTEON(const user_osc_param_t * const params)
{
    (void)params;
    if (!s.noteDown)
    {
        s.attackPhaseComplete = false;
    }
    s.noteDown = true;
}

void OSC_NOTEOFF(const user_osc_param_t * const params)
{
    (void)params;
    s.noteDown = false;
    s.releasePhaseComplete = false;
}

void OSC_PARAM(uint16_t index, uint16_t value)
{
    switch (index)
    {
        case k_user_osc_param_id1: // Attack
            s.attackResistance = value * ATTACK_MULT + 1;
            break;

        case k_user_osc_param_id2: // Release
            {
                s.releaseFactor = value - 100;
                calculateReleaseResistance();
            }
            break;

        case k_user_osc_param_shape: // Decay
            s.decayResistance = param_val_to_f32(value) * DECAY_MULT;
            s.decayResistance++;
            calculateReleaseResistance(); // release is based on decay, so recalculate it here
            break;

        case k_user_osc_param_shiftshape: // Sustain
            s.sustainLevel = param_val_to_f32(value);
            break;

        default:
            break;
    }
}

void nothing()
{
    return;
}
