#pragma once

/*
 * File: noisegen.hpp
 *
 * Noise generator interface
 *
 */

class NoiseGenerator
{
public:
    virtual void Tick() = 0;
    virtual float GetValue() = 0;

    virtual ~NoiseGenerator() {};
};


