#pragma once

/*
 * File: red.hpp
 *
 * Red (brownian) noise generator
 *
 */

#include "noisegen.hpp"

class RedNoise : public NoiseGenerator
{
public:
    RedNoise();

    virtual void Tick() override;
    virtual float GetValue() override;
    
    virtual ~RedNoise() override {};

private:
    float buffer;
};
