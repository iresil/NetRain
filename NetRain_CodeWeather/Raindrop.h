#pragma once
#include "Droplet.h"

namespace N_CodeRain
{
    class Raindrop
    {
    public:
        Raindrop(int tail_length, int fall_seconds_multiplier,
            int** symbols, int change_seconds_multiplier[], float opacity[]);
        Droplet** inspect_droplets();
        void fall();
    private:
        int droplet_offset;
        int tail_length;
        int fall_seconds_multiplier;
        Droplet** drops;
    };
}
