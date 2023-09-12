#pragma once

#include "../../NetRain_Resources/ResourceHandler.h"
#include "../../NetRain_CodeWeather/CodeCloud.h"

namespace N_CodeRain
{
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace System::Collections::Generic;

    class CodeRain
    {
    public:
        ref class Managed
        {
        public:
            static List<Bitmap^>^ images = nullptr;
        };

        static CodeRain* getInstance();
        void paint(PictureBox^ codeRainBox, PaintEventArgs^ e);
    private:
        CodeRain(int raindrops);
        static CodeRain* instancePtr;

        ~CodeRain();
        CodeRain(const CodeRain&) = delete;
        CodeRain(CodeRain& other) = delete;
        CodeRain& operator=(const CodeRain&) = delete;

        N_CodeRain_Res::ResourceHandler * resourceHandler;
        char** vectors;

        CodeCloud* codeCloud;
        int raindrops;

        Bitmap^ resourceToBitmap(char* res_str);
        void paintFromCloud(List<Bitmap^>^ images, PictureBox^ codeRainBox, PaintEventArgs^ e);
    };
}
