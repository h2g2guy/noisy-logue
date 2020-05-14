
/*
 * File: noisy.cpp
 *
 * Enveloped noise generator
 *
 */

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

#include "noisegen.hpp"
#include "white.hpp"
#include "pink.hpp"
#include "red.hpp"
#include "decim.hpp"
#include "state.hpp"
#include "noisegenmanager.hpp"

extern "C" int __aeabi_atexit(
        void *object,
        void (*destructor)(void *),
        void *dso_handle)
{
    static_cast<void>(object);
    static_cast<void>(destructor);
    static_cast<void>(dso_handle);
    return 0;
}

extern "C" void operator delete(void*)
{
    return;
}

void* __dso_handle = nullptr;

static State s = State();
static NoiseGenManager noiseGens(&s);

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
        float multiplier = ((float)s.releaseFactor + 100.f) / 100.f;
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
    s.sustainLevel = 0.f;
    s.releaseFactor = 0;
    calculateReleaseResistance();
    s.releaseFactor = 101; //set to an impossible sentinel value to workaround weird prologue bug

    s.noteDown = false;
    s.attackPhaseComplete = false;

    s.noiseType = NOISETYPE_WHITE;

    s.modValue = 101; //set to an impossible sentinel value to workaround weird prologue bug

    s.lfoTarget = TARGETSELECT_VOLUME;
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
        // release/off phase
        s.currentLevel = clip0f(s.currentLevel + getChangeInLevel(0.f, s.currentLevel, s.releaseResistance));
    }
}

float genNoise()
{
    noiseGens[s.noiseType]->Tick();
    return noiseGens[s.noiseType]->GetValue();
}

void OSC_CYCLE(const user_osc_param_t * const params,
        int32_t *yn,
        const uint32_t frames)
{
    float output = 0.f;

    for (uint32_t i = 0; i < frames; i++)
    {
        // Generate noise of the right type
        output = genNoise();

        // Get level, modify it, and apply it
        getNewLevel();
        float level = s.currentLevel;
        if (s.lfoTarget & TARGETSELECT_VOLUME)
        {
            level = clip01f(level + q31_to_f32(params->shape_lfo));
        }
        output *= level;

        // Output
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
}

void OSC_PARAM(uint16_t index, uint16_t value)
{
    switch (index)
    {
        case k_user_osc_param_shape: // Decay
            s.decayResistance = param_val_to_f32(value) * DECAY_MULT;
            s.decayResistance++;
            calculateReleaseResistance(); // release is based on decay, so recalculate it here
            break;

        case k_user_osc_param_shiftshape: // Sustain
            s.sustainLevel = param_val_to_f32(value);
            break;

        case k_user_osc_param_id1: // Attack
            s.attackResistance = value * ATTACK_MULT + 1;
            break;

        case k_user_osc_param_id2: // Release
            {
                if (value == 0 && s.releaseFactor == 101)
                {
                    // seems to be a weird case where, on initialization, stuff gets forcibly set to minimum when it
                    // should instead be the correctly-scaled zero point. account for that case here.
                    s.releaseFactor = 0;
                    calculateReleaseResistance();
                    break;
                }
                s.releaseFactor = value - 100;
                calculateReleaseResistance();
            }
            break;

        case k_user_osc_param_id3: // Noise Type
            s.noiseType = value;
            break;

        case k_user_osc_param_id4: // Noise mod
            {
                if (value == 0 && s.modValue == 101)
                {
                    // seems to be a weird case where, on initialization, stuff gets forcibly set to minimum when it
                    // should instead be the correctly-scaled zero point. account for that case here.
                    s.modValue = 0;
                    break;
                }
                s.modValue = value - 100;
                calculateReleaseResistance();
            }
            break;

        case k_user_osc_param_id5: // LFO target
            s.lfoTarget = value;
            break;


        default:
            break;
    }
}

