#include "pch.h"
#include "Resource.h"
#include "ResourceHandler.h"
#include "ResourceItem.h"

namespace N_CodeRain_Res
{
    ResourceHandler::ResourceHandler()
    {
        num_res = 0;
    }

    char** ResourceHandler::GetAllVectors()
    {
        int first_vector = IDR_VECTOR1;
        LONG_PTR hResult = 0;

        HMODULE hModule = NULL;
        GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            L"NetRain_Resources", &hModule);

        LONG_PTR test = (LONG_PTR)this;
        EnumResourceNamesW(hModule, L"Vector",
            (ENUMRESNAMEPROCW)[](HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam) {
                ResourceHandler* this_res = (ResourceHandler*)lParam;
                (*this_res).num_res++;
                return TRUE;
            }, (LONG_PTR)this);
        char** result = new char* [num_res + 1];
        int vector = first_vector;
        for (int i = 1; i <= num_res; i++)
        {
            ResourceItem res(vector, L"Vector");
            result[i - 1] = res.GetResourceString();
            vector++;
        }
        result[num_res] = nullptr;
        return result;
    }
}
