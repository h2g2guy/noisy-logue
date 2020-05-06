
/*
 * File: white.cpp
 *
 * White noise generator; wrapper of osc_white
 *
 */


#include "userosc.h"
#include "white.hpp"

WhiteNoise::WhiteNoise(State* s) : NoiseGenerator(s)
{
    Tick();
}

void WhiteNoise::Tick()
{
    buffer = osc_white();
}

float WhiteNoise::GetValue()
{
    return buffer;
}
