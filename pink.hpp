#pragma once

/*
 * File: pink.hpp
 *
 * Pink noise generator from white noise
 *
 */

#define PINK_NUM_BUFFERS 8

#include "noisegen.hpp"

class PinkNoise : public NoiseGenerator
{
public:
    PinkNoise();

    virtual void Tick() override;
    virtual float GetValue() override;

    virtual ~PinkNoise() override {};

private:
    float buffers[PINK_NUM_BUFFERS];
    unsigned int tick;

};

