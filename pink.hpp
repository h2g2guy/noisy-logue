#pragma once

/*
 * File: pink.hpp
 *
 * Pink noise generator from white noise
 *
 */

#include "noisegen.hpp"
#include "state.hpp"
#include "twopole.hpp"

class PinkNoise : public NoiseGenerator
{
public:
    PinkNoise(State* s);

    virtual void Tick() override;
    virtual float GetValue() override;

    virtual ~PinkNoise() override {};

private:
    static constexpr size_t numberOfBuffers = 8;
    float buffers[numberOfBuffers];
    unsigned int tick;
    Filter filter;
};

