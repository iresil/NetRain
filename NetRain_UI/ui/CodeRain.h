#pragma once

namespace N_CodeRain
{
    using namespace System::Drawing;
    using namespace System::Windows::Forms;
    using namespace System::Collections::Generic;

    class CodeRain
    {
    public:
        static void paint(PictureBox^ codeRainBox, PaintEventArgs^ e);
    private:
        static Bitmap^ resourceToBitmap(char* res_str);
        static void paintImageGrid(List<Bitmap^>^ images, PictureBox^ codeRainBox, PaintEventArgs^ e);
    };
}
