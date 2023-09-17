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
        int get_tail_length();
        int get_droplet_offset();
        int get_fall_seconds_multiplier();
        void reset_raindrop(int tail_length, int fall_seconds_mult, int* change_seconds_multiplier,
            float* opacity, int** symbols, int rows);
        void fall();
    private:
        int droplet_offset;
        int tail_length;
        int fall_seconds_multiplier;
        int times_fallen;
        Droplet** drops;
    };
}
