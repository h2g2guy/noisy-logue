#pragma once

/*
 * File: pink.hpp
 *
 * Pink noise generator from white noise
 *
 */

#define PINK_NUM_BUFFERS 8

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
    float buffers[PINK_NUM_BUFFERS];
    unsigned int tick;
    Filter filter;
};

