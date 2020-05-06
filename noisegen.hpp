#pragma once

/*
 * File: noisegen.hpp
 *
 * Noise generator interface
 *
 */

struct State;

class NoiseGenerator
{
public:
    NoiseGenerator(State* s)
    {
        state = s;
    }

    virtual void Tick() = 0;
    virtual float GetValue() = 0;

    virtual ~NoiseGenerator() {};

protected:
    State* state;
};


