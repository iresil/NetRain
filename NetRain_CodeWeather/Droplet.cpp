#include "pch.h"
#include "Droplet.h"

namespace N_CodeRain
{
    Droplet::Droplet(int change_seconds_multiplier, float opacity, int symbols[])
    {
        this->symbols = symbols;
        this->opacity = opacity;
        this->change_seconds_multiplier = change_seconds_multiplier;
    }
}
