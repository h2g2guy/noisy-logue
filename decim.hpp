#pragma once

/*
 * File: decim.hpp
 *
 * Decimating noise generator
 *
 */

#include "noisegen.hpp"
#include "state.hpp"

struct State;

class DecimatedNoise : public NoiseGenerator
{
public:
    DecimatedNoise(State* s);

    virtual void Tick() override;
    virtual float GetValue() override;

    virtual ~DecimatedNoise() override {};

private:
    float buffer;
    int tick;
};
