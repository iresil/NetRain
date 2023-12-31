#include "pch.h"
#include "Raindrop.h"
#include "../NetRain_Common/Consts.h"

namespace N_CodeRain
{
    Raindrop::Raindrop(int tail_length, int fall_seconds_multiplier,
        int** symbols, int change_seconds_multiplier[], float opacity[])
    {
        this->droplet_offset = (rand() % (RAINDROP_MAX_TAIL_SIZE * 2)) - RAINDROP_MAX_TAIL_SIZE;
        this->tail_length = tail_length;
        this->fall_seconds_multiplier = fall_seconds_multiplier;
        this->times_fallen = 0;
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

    int Raindrop::get_tail_length()
    {
        return this->tail_length;
    }

    int Raindrop::get_droplet_offset()
    {
        return this->droplet_offset;
    }

    int Raindrop::get_fall_seconds_multiplier()
    {
        return this->fall_seconds_multiplier;
    }

    void Raindrop::fall()
    {
        this->times_fallen++;
        if (times_fallen >= fall_seconds_multiplier)
        {
            this->times_fallen = 0;
            this->droplet_offset++;
        }
    }

    void Raindrop::reset_raindrop(int tail_length, int fall_seconds_mult, int* change_seconds_multiplier,
        float* opacity, int** symbols, int rows)
    {
        for (int i = 0; i < this->tail_length; i++)
        {
            delete this->drops[i];
        }
        delete[] this->drops;
        
        this->tail_length = tail_length;
        
        Droplet** droplet_array = new Droplet* [tail_length + 1];
        for (int i = 0; i < tail_length; i++)
        {
            *(droplet_array + i) = new Droplet(change_seconds_multiplier[i], opacity[i], symbols[i]);
        }
        droplet_array[tail_length] = NULL;
        this->drops = droplet_array;

        this->fall_seconds_multiplier = fall_seconds_mult;
        this->droplet_offset = (rand() % RAINDROP_MAX_TAIL_SIZE) - rows - this->tail_length;
    }
}
