#pragma once

#include "../../NetRain_CodeWeather/CodeCloud.h"

namespace N_CodeRain
{
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace System::Collections::Generic;

    class CodeRain
    {
    public:
        static CodeRain& getInstance();
        CodeRain(int raindrops);
        void paint(PictureBox^ codeRainBox, PaintEventArgs^ e);
    private:
        CodeCloud* codeCloud;
        int raindrops;
        Bitmap^ resourceToBitmap(char* res_str);
        void paintFromCloud(List<Bitmap^>^ images, PictureBox^ codeRainBox, PaintEventArgs^ e);

        ~CodeRain();
        CodeRain(const CodeRain&) = delete;
        CodeRain& operator=(const CodeRain&) = delete;
    };
}
