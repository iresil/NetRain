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
            static void ReleaseResources();
            static void ReleaseBitmaps();
            static void ReleaseBitmapAt(List<Bitmap^>^ list, int index);

            static List<Bitmap^>^ getImages(int offs, bool preparationSuccess);
            static Color getDropletOutline(int offs, bool preparationSuccess);
            static Color getDropletInner(int offs, bool preparationSuccess);
            static Color getDropletFirst(int offs, bool preparationSuccess);

            static void setImages(List<List<List<Bitmap^>^>^>^ images);
            static void setDropletOutline(int offs, Color color, Color color_alt);
            static void setDropletInner(int offs, Color color, Color color_alt);
            static void setDropletFirst(int offs, Color color, Color color_alt);
        private:
            static List<List<Bitmap^>^>^ images = nullptr;
            static List<Color>^ droplet_outline = nullptr;
            static List<Color>^ droplet_inner = nullptr;
            static List<Color>^ droplet_first = nullptr;

            static List<List<Bitmap^>^>^ images_alt = nullptr;
            static List<Color>^ droplet_outline_alt = nullptr;
            static List<Color>^ droplet_inner_alt = nullptr;
            static List<Color>^ droplet_first_alt = nullptr;
        };

        static CodeRain* getInstance();
        void paint(PictureBox^ codeRainBox, PaintEventArgs^ e);
    private:
        CodeRain(int raindrops);
        static CodeRain* instancePtr;
        bool preparationSuccess;

        ~CodeRain();
        CodeRain(const CodeRain&) = delete;
        CodeRain(CodeRain& other) = delete;
        CodeRain& operator=(const CodeRain&) = delete;

        N_CodeRain_Res::ResourceHandler * resourceHandler;
        char** vectors;

        NetToRaindropParams* netToRaindrop;
        CodeCloud* codeCloud[2];
        int raindrops;

        Bitmap^ resourceToBitmap(char* res_str, int offs, bool preparationSuccess);
        List<List<List<Bitmap^>^>^>^ makeImages(char** vectors);
        void paintFromCloud(int offs, PictureBox^ codeRainBox, PaintEventArgs^ e);
    };
}
