#pragma once

namespace N_CodeRain
{
    class Droplet
    {
    public:
        Droplet(int change_seconds_multiplier, float opacity, int symbols[]);
        int* get_symbols();
        int get_next_symbol(int fall_seconds_multiplier);
        float get_opacity();
    private:
        int change_seconds_multiplier;
        float opacity;
        int* symbols;
        int current_symbol_index;
        int times_changed;
    };
}
