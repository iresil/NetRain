#pragma once

#include <wtypes.h>

namespace N_CodeRain
{
    struct Parameters {
        DWORD size_bytes = 0;
        LPVOID ptr = nullptr;
    };

    class ResourceHandler {
    public:
        ResourceHandler(int resource_id, const char* resource_class);
        Parameters GetResource() const;
        char* GetResourceString() const;
    private:
        HRSRC hResource = nullptr;
        HGLOBAL hMemory = nullptr;
        Parameters p;
    };
}
