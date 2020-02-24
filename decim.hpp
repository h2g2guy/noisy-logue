#pragma once

/*
 * File: decim.hpp
 *
 * Decimating noise generator
 *
 */

class DecimatedNoise
{
public:
    DecimatedNoise();

    void Tick();
    float GetValue();
    void SetDecimationFactor(int factor);

private:
    float buffer;
    unsigned int tick;
    unsigned int factor;
};
