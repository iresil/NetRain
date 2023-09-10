#include "pch.h"
#include "Raindrop.h"

namespace N_CodeRain
{
    Raindrop::Raindrop(int tail_length, int fall_seconds_multiplier,
        int** symbols, int change_seconds_multiplier[], float opacity[])
    {
        this->droplet_offset = 0;
        this->tail_length = tail_length;
        this->fall_seconds_multiplier = fall_seconds_multiplier;
        Droplet** droplet_array = new Droplet* [tail_length + 1];
        for (int i = 0; i < tail_length; i++)
        {
            *(droplet_array + i) = new Droplet(change_seconds_multiplier[i], opacity[i], symbols[i]);
        }
        droplet_array[tail_length] = NULL;
        this->drops = droplet_array;
    }

    Droplet** Raindrop::inspect_droplets()
    {
        return this->drops;
    }

    void Raindrop::fall()
    {
        this->droplet_offset++;
    }
}
