#pragma once

namespace N_CodeRain_Res
{
    class ResourceHandler
    {
    public:
        ResourceHandler();
        char** GetAllVectors();
    private:
        int num_res;
    };
}
