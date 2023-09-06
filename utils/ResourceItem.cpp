#include "..\pch.h"
#include "ResourceItem.h"
#include <wtypes.h>

namespace N_CodeRain {
    ResourceItem::ResourceItem(int resource_id, LPCWSTR resource_class) {
        hResource = FindResource(nullptr, MAKEINTRESOURCE(resource_id), resource_class);

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

    Parameters ResourceItem::GetResource() const {
        return p;
    }

    char* ResourceItem::GetResourceString() const {
        char* dst = NULL;
        if (p.ptr != nullptr)
            dst = reinterpret_cast<char*>(p.ptr);
        return dst;
    }
}
