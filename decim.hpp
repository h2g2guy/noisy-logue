#pragma once

/*
 * File: decim.hpp
 *
 * Decimating noise generator
 *
 */

#include "noisegen.hpp"

class DecimatedNoise : public NoiseGenerator
{
public:
    DecimatedNoise();

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
