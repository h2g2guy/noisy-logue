
/*
 * File: noisy.cpp
 *
 * Enveloped noise generator
 *
 */

#include "noisegen.hpp"
#include "white.hpp"
#include "pink.hpp"
#include "red.hpp"
#include "decim.hpp"
#include "state.hpp"
#include "noisegenmanager.hpp"

// Attack and release multipliers are relative to menu items, which are 0-100
// Decay multiplier is relative to the Shape knob, which is a q31
constexpr int32_t attackMultiplier = 480;
constexpr float decayMultiplier = 48000.f;
constexpr int32_t releaseMultiplierPositive = 480;

constexpr float capacitance = 1.f;
constexpr float targetLevelOffset = 0.01f;

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
        s.releaseResistance = s.releaseFactor * releaseMultiplierPositive + s.decayResistance;
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
    s.lfoLevel = 0.f;

    s.attackResistance = 1;
    s.decayResistance = 1;
    s.sustainLevel = 0.f;
    s.releaseFactor = 0;
    calculateReleaseResistance();
    s.releaseFactor = 0;

    s.noteDown = false;
    s.attackPhaseComplete = false;

    s.noiseType = static_cast<int32_t>(NoiseType::White);

    s.modValue = 0;

    s.lfoTarget = 1;
    s.envToModPercentage = 0;
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
        offsetTarget = targetLevel - targetLevelOffset;
        return currentLevel > offsetTarget;
    }
    else
    {
        offsetTarget = targetLevel + targetLevelOffset;
        return currentLevel < offsetTarget;
    }
}

float getChangeInLevel(float targetLevel, float currentLevel, float resistance)
{
    return (targetLevel / resistance) - (currentLevel / (resistance * capacitance));
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
    s.lfoLevel = q31_to_f32(params->shape_lfo);

    float output = 0.f;
    float level = 0.f;

    for (uint32_t i = 0; i < frames; i++)
    {
        output = genNoise();

        getNewLevel();

        if (s.envToModPercentage == 0)
        {
            // apply the envelope to the signal
            level = s.currentLevel;

            // apply lfo if present
            if (s.lfoTarget & LfoTargetFlags::Volume)
            {
                level = clip01f(level + s.lfoLevel);
            }
        }
        else
        {
            // envelope is being applied to noise mod; ignore it here

            if (s.lfoTarget & LfoTargetFlags::NoiseMod)
            {
                // center the noise level
                level = clip01f(0.5f + (s.lfoLevel / 0.5f));
            }
            else
            {
                level = 1.f;
            }
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

void setBipolarValueWithWorkaround(uint16_t value, int32_t& target)
{
    if (value == 0)
    {
        // seems to be a weird case where, on initialization, stuff gets forcibly set to minimum when it
        // should instead be the correctly-scaled zero point. account for that case here.
        target = 0;
        return;
    }
    target = value - 100;
}

void OSC_PARAM(uint16_t index, uint16_t value)
{
    switch (index)
    {
        case k_user_osc_param_shape: // Decay
            s.decayResistance = param_val_to_f32(value) * decayMultiplier;
            s.decayResistance++;
            calculateReleaseResistance(); // release is based on decay, so recalculate it here
            break;

        case k_user_osc_param_shiftshape: // Sustain
            s.sustainLevel = param_val_to_f32(value);
            break;

        case k_user_osc_param_id1: // Attack
            s.attackResistance = value * attackMultiplier + 1;
            break;

        case k_user_osc_param_id2: // Release
            setBipolarValueWithWorkaround(value, s.releaseFactor);
            calculateReleaseResistance();
            break;

        case k_user_osc_param_id3: // Noise Type
            s.noiseType = value;
            break;

        case k_user_osc_param_id4: // Noise mod
            setBipolarValueWithWorkaround(value, s.modValue);
            break;

        case k_user_osc_param_id5: // LFO target
            s.lfoTarget = value;
            break;

        case k_user_osc_param_id6: // Env -> Mod
            setBipolarValueWithWorkaround(value, s.envToModPercentage);
            break;

        default:
            break;
    }
}

