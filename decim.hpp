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

    // make this into a generic version eventually, once other noise generators are modulatable l
    void SetDecimationFactor(int factor);

private:
    float buffer;
    unsigned int tick;
    unsigned int factor;
};
