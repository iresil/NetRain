#pragma once

namespace N_CodeRain
{
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    class CodeRain {
    public:
        static void paint(PictureBox^ codeRainBox, PaintEventArgs^ e);
    private:
        static void paintImageGrid(Bitmap^ image, PictureBox^ codeRainBox, PaintEventArgs^ e);
        static Bitmap^ resourceToBitmap(int resource_id);
    };
}
