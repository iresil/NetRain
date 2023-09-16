#pragma once

#include "../NetToRaindropParams.h"
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
            static List<List<Bitmap^>^>^ images = nullptr;
            static List<Color>^ droplet_default_outline = nullptr;
            static List<Color>^ droplet_default_inner = nullptr;
            static List<Color>^ droplet_inner = nullptr;
            static List<Color>^ droplet_glow = nullptr;
            static List<Color>^ droplet_first = nullptr;
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

        NetToRaindropParams* netToRaindrop;
        CodeCloud* codeCloud[2];
        int raindrops;

        Bitmap^ resourceToBitmap(char* res_str, int offs);
        void paintFromCloud(int offs, PictureBox^ codeRainBox, PaintEventArgs^ e);
    };
}
