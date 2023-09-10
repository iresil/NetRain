#pragma once

namespace N_CodeRain_Res
{
    struct Parameters {
        DWORD size_bytes = 0;
        LPVOID ptr = nullptr;
    };

    class ResourceItem {
    public:
        ResourceItem(int resource_id, LPCWSTR resource_class);
        Parameters GetResource() const;
        char* GetResourceString() const;
    private:
        HRSRC hResource = nullptr;
        HGLOBAL hMemory = nullptr;
        Parameters p;
        HMODULE GetCurrentModule();
    };
}
