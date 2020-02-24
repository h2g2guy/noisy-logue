#pragma once

/*
 * File: pink.hpp
 *
 * Pink noise generator from white noise
 *
 */

#define PINK_NUM_BUFFERS 8

class PinkNoise
{
public:
    PinkNoise();

    void Tick();
    float GetValue();

private:
    float buffers[PINK_NUM_BUFFERS];
    unsigned int tick;

};

