#pragma once

/*
 * File: white.hpp
 *
 * White noise generator; wrapper for osc_white
 *
 */

#include "noisegen.hpp"
#include "state.hpp"

class WhiteNoise : public NoiseGenerator
{
public:
    WhiteNoise(State* s);

    virtual void Tick() override;
    virtual float GetValue() override;

    virtual ~WhiteNoise() override {};

private:
    float buffer;
};

