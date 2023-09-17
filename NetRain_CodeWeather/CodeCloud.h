#pragma once
#include "Raindrop.h"

namespace N_CodeRain
{
    class CodeCloud
    {
    public:
        CodeCloud(int raindrops);

        Raindrop** inspect_raindrops();
        void reset_raindrop(int raindrop, int rows, int tail_length = -1);

        void MakeItRain();
    private:
        int raindrop_count;
        Raindrop** raindrops;

        Raindrop* generate_raindrop_rand();

        void generate_raindrop_params(int& tail_length, int& fall_seconds_mult, int** &symbols,
            int* &change_seconds_mult, float* &opacity);
    };
}
