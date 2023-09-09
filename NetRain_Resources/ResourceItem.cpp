#include "pch.h"
#include "ResourceItem.h"

namespace N_CodeRain_Res {
    ResourceItem::ResourceItem(int resource_id, LPCWSTR resource_class) {
        HMODULE hModule = GetCurrentModule();
        hResource = FindResource(hModule, MAKEINTRESOURCE(resource_id), resource_class);

        if (hResource != 0)
        {
            hMemory = LoadResource(hModule, hResource);
            if (hMemory != 0)
            {
                p.size_bytes = SizeofResource(hModule, hResource);
                p.ptr = LockResource(hMemory);
            }
        }
    }

    HMODULE ResourceItem::GetCurrentModule()
    {
        HMODULE hModule = NULL;
        GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            L"NetRain_Resources", &hModule);

        return hModule;
    }

    Parameters ResourceItem::GetResource() const
    {
        return p;
    }

    char* ResourceItem::GetResourceString() const
    {
        char* dst = NULL;
        if (p.ptr != nullptr)
            dst = reinterpret_cast<char*>(p.ptr);
        return dst;
    }
}
