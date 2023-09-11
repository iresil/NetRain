#include "pch.h"
#include "CodeCloud.h"

namespace N_CodeRain
{
    CodeCloud::CodeCloud(int raindrop_count)
    {
        this->raindrop_count = raindrop_count;
        Raindrop** raindrops = new Raindrop* [raindrop_count + 1];
        for (int i = 0; i < raindrop_count; i++)
        {
            int tail_length_rand = 10 + rand() % (30 - 10);
            int fall_seconds_mult_rand = 1 + rand() % (4 - 1);
            int drop_count = 6;
            int** symbols_rand = new int*[tail_length_rand + 1];
            int* change_seconds_mult = new int[tail_length_rand + 1];
            float* opacity = new float[tail_length_rand + 1];
            for (int j = 0; j < tail_length_rand; j++)
            {
                int* symbols_tmp = new int[drop_count + 1];
                for (int k = 0; k < drop_count; k++)
                {
                    symbols_tmp[k] = 1 + rand() % (55 - 1);
                }
                symbols_tmp[drop_count] = NULL;
                symbols_rand[j] = *&symbols_tmp;

                change_seconds_mult[j] = j + 1;

                opacity[j] = (drop_count / 100.0) * (tail_length_rand - j);
            }
            symbols_rand[tail_length_rand] = NULL;
            change_seconds_mult[tail_length_rand] = NULL;
            opacity[tail_length_rand] = NULL;
            *(raindrops + i) = new Raindrop(tail_length_rand, fall_seconds_mult_rand,
                symbols_rand, change_seconds_mult, opacity);
        }
        raindrops[raindrop_count] = NULL;
        this->raindrops = raindrops;
    }

    Raindrop** CodeCloud::inspect_raindrops()
    {
        return this->raindrops;
    }

    void CodeCloud::MakeItRain()
    {
        for (int i = 0; i < this->raindrop_count; i++)
        {
            raindrops[i]->fall();
        }
    }
}
