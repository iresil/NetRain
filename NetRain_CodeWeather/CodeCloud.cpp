#include "pch.h"
#include "CodeCloud.h"
#include "../NetRain_Common/Consts.h"

namespace N_CodeRain
{
    CodeCloud::CodeCloud(int raindrop_count)
    {
        this->raindrop_count = raindrop_count;
        Raindrop** raindrops = new Raindrop* [raindrop_count + 1];
        for (int i = 0; i < raindrop_count; i++)
        {
            *(raindrops + i) = generate_raindrop_rand();
        }
        raindrops[raindrop_count] = NULL;
        this->raindrops = raindrops;
    }

    Raindrop* CodeCloud::generate_raindrop_rand()
    {
        int tail_length = rand() % 3;
        int fall_seconds_mult = IGNORE_VALUE;
        int** symbols = nullptr;
        int* change_seconds_mult = nullptr;
        float* opacity = nullptr;

        generate_raindrop_params(tail_length, fall_seconds_mult, symbols, change_seconds_mult, opacity);

        return new Raindrop(tail_length, fall_seconds_mult, symbols, change_seconds_mult, opacity);
    }

    void CodeCloud::generate_raindrop_params(int& tail_length, int& fall_seconds_mult, int** &symbols,
        int* &change_seconds_mult, float* &opacity)
    {
        tail_length = (tail_length > IGNORE_VALUE) ? tail_length : RAINDROP_MIN_TAIL_SIZE + rand() % (RAINDROP_MAX_TAIL_SIZE - RAINDROP_MIN_TAIL_SIZE);
        fall_seconds_mult = (fall_seconds_mult > IGNORE_VALUE) ? fall_seconds_mult : RAINDROP_MIN_FALL_SECONDS_MULT + rand() % (RAINDROP_MAX_FALL_SECONDS_MULT - RAINDROP_MIN_FALL_SECONDS_MULT);
        int drop_count = DROPLET_LETTER_COUNT;
        int** symbols_new = new int* [tail_length + 1];
        int* change_seconds_mult_new = new int[tail_length + 1];
        float* opacity_new = new float[tail_length + 1];
        for (int j = 0; j < tail_length; j++)
        {
            int* symbols_tmp = new int[drop_count + 1];
            for (int k = 0; k < drop_count; k++)
            {
                symbols_tmp[k] = DROPLET_RANDOM_LETTER_OFFSET_MIN + rand() % (DROPLET_RANDOM_LETTER_OFFSET_MAX - DROPLET_RANDOM_LETTER_OFFSET_MIN);
            }
            symbols_tmp[drop_count] = NULL;
            symbols_new[j] = *&symbols_tmp;

            change_seconds_mult_new[j] = j + 1;

            opacity_new[j] = (drop_count / 100.0) * (tail_length - j);
        }
        symbols_new[tail_length] = NULL;
        change_seconds_mult_new[tail_length] = NULL;
        opacity_new[tail_length] = NULL;

        if (symbols == nullptr)
        {
            symbols = symbols_new;
        }
        else
        {
            for (int i = 0; i < tail_length; i++)
            {
                delete[] symbols_new[i];
            }
            delete[] symbols_new;
        }

        if (change_seconds_mult == nullptr)
        {
            change_seconds_mult = change_seconds_mult_new;
        }
        else
        {
            delete[] change_seconds_mult_new;
        }

        if (opacity == nullptr)
        {
            opacity = opacity_new;
        }
        else
        {
            delete[] opacity_new;
        }
    }

    Raindrop** CodeCloud::inspect_raindrops()
    {
        return this->raindrops;
    }

    void CodeCloud::reset_raindrop(int raindrop, int rows, int tail_length)
    {
        int fall_seconds_mult = IGNORE_VALUE;
        int** symbols = nullptr;
        int* change_seconds_mult = nullptr;
        float* opacity = nullptr;

        generate_raindrop_params(tail_length, fall_seconds_mult, symbols, change_seconds_mult, opacity);

        this->raindrops[raindrop]->reset_raindrop(tail_length, fall_seconds_mult, change_seconds_mult, opacity, symbols, rows);

        delete[] change_seconds_mult;
        delete[] opacity;
        delete[] symbols;
    }

    void CodeCloud::MakeItRain()
    {
        for (int i = 0; i < this->raindrop_count; i++)
        {
            raindrops[i]->fall();
        }
    }
}
