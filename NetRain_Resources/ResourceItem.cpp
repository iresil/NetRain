#include "pch.h"
#include "ResourceItem.h"

namespace N_CodeRain_Res
{
    ResourceItem::ResourceItem(int resource_id, LPCWSTR resource_class)
    {
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
        char* res_str = NULL;
        char* res_str_nterm = NULL;
        size_t str_len = 0;
        if (p.ptr != nullptr)
        {
            str_len = p.size_bytes;
            size_t str_len_nterm = str_len + 1;
            res_str = reinterpret_cast<char*>(p.ptr);

            res_str_nterm = new char[str_len_nterm];
            strcpy_s(res_str_nterm, str_len_nterm, res_str);
            res_str_nterm[str_len_nterm] = NULL;
        }
        return res_str_nterm;
    }
}
