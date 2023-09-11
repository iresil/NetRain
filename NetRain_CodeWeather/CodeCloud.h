#pragma once
#include "Raindrop.h"

namespace N_CodeRain
{
    class CodeCloud
    {
    public:
        CodeCloud(int raindrops);
        Raindrop** inspect_raindrops();
        void MakeItRain();
    private:
        int raindrop_count;
        Raindrop** raindrops;
    };
}
