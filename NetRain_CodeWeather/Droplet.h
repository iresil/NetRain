#pragma once

namespace N_CodeRain
{
    class Droplet
    {
    public:
        Droplet(int change_seconds_multiplier, float opacity, int symbols[]);
    private:
        int change_seconds_multiplier;
        float opacity;
        int* symbols;
    };
}
