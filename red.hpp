#pragma once

/*
 * File: red.hpp
 *
 * Red (brownian) noise generator
 *
 */

#include "noisegen.hpp"
#include "state.hpp"
#include "twopole.hpp"

class RedNoise : public NoiseGenerator
{
public:
    RedNoise(State* s);

    virtual void Tick() override;
    virtual float GetValue() override;

    virtual ~RedNoise() override {};

private:
    float buffer;
    Filter filter;
};
