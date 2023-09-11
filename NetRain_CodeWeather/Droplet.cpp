#include "pch.h"
#include "Droplet.h"

namespace N_CodeRain
{
    Droplet::Droplet(int change_seconds_multiplier, float opacity, int symbols[])
    {
        this->symbols = symbols;
        this->current_symbol_index = 0;
        this->times_changed = 0;
        this->opacity = opacity;
        this->change_seconds_multiplier = change_seconds_multiplier;
    }

    float Droplet::get_opacity()
    {
        return this->opacity;
    }

    int* Droplet::get_symbols()
    {
        return this->symbols;
    }

    int Droplet::get_next_symbol(int fall_seconds_multiplier)
    {
        times_changed++;
        int symbol = this->symbols[this->current_symbol_index];
        if (times_changed * 2 >= change_seconds_multiplier + fall_seconds_multiplier)
        {
            this->times_changed = 0;
            this->current_symbol_index++;
            if (this->symbols[this->current_symbol_index] == NULL)
            {
                this->current_symbol_index = 0;
            }
        }
        return symbol;
    }
}
