#pragma once
#include "Raindrop.h"

namespace N_CodeRain
{
    class CodeCloud
    {
    public:
        CodeCloud(int raindrops);

        Raindrop** inspect_raindrops();
        void reset_raindrop(int raindrop, int tail_length, int rows);

        void MakeItRain();
    private:
        int raindrop_count;
        Raindrop** raindrops;

        Raindrop* generate_raindrop_rand();
    };
}
