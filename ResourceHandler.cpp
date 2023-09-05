#include "pch.h"
#include "ResourceHandler.h"
#include <wtypes.h>

namespace N_CodeRain {
    ResourceHandler::ResourceHandler(int resource_id, const char* resource_class) {
        hResource = FindResource(nullptr, MAKEINTRESOURCE(resource_id), RT_STRING);
        if (hResource != 0)
        {
            hMemory = LoadResource(nullptr, hResource);
            if (hMemory != 0)
            {
                p.size_bytes = SizeofResource(nullptr, hResource);
                p.ptr = LockResource(hMemory);
            }
        }
    }

    Parameters ResourceHandler::GetResource() const {
        return p;
    }

    char* ResourceHandler::GetResourceString() const {
        char* dst = NULL;
        if (p.ptr != nullptr)
            dst = reinterpret_cast<char*>(p.ptr);
        return dst;
    }
}
