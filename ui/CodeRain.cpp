#include "..\pch.h"
#include "CodeRain.h"
#include "..\Resource.h"
#include "..\utils\ResourceItem.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"

namespace N_CodeRain
{
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Drawing::Imaging;
    using namespace System::Drawing::Drawing2D;
    using namespace System::Collections::Generic;

    void CodeRain::paint(PictureBox^ codeRainBox, PaintEventArgs^ e)
    {
        e->Graphics->SmoothingMode = SmoothingMode::HighQuality;

        GraphicsPath^ gpath = gcnew GraphicsPath(FillMode::Winding);
        Region^ reg = gcnew Region();
        Pen^ pen = gcnew Pen(Brushes::Black);

        e->Graphics->FillRectangle(Brushes::Black, 0, 0, codeRainBox->Width, codeRainBox->Height);

        Bitmap^ bmp = CodeRain::resourceToBitmap(IDR_VECTOR54);
        CodeRain::paintImageGrid(bmp, codeRainBox, e);
        delete bmp;
    }

    Bitmap^ CodeRain::resourceToBitmap(int resource_id)
    {
        ResourceItem res(resource_id, L"Vector");
        char* res_str = res.GetResourceString();

        // Change resource string to null terminated
        size_t str_len = strlen(res_str);
        size_t str_len_nterm = str_len + 1;
        char* res_str_nterm = new char[str_len_nterm];
        strcpy(res_str_nterm, res_str);
        res_str_nterm[str_len_nterm] = '\0';

        struct NSVGimage* image = nsvgParse(res_str_nterm, "px", 96);

        Bitmap^ bmp = gcnew Bitmap(image->width, image->height);
        GraphicsPath^ gpath = gcnew GraphicsPath(FillMode::Winding); 
        Region^ reg = gcnew Region(); 
        Pen^ pen = gcnew Pen(Brushes::Black);
        Graphics^ graphics = Graphics::FromImage(bmp);

        for (NSVGshape* shape = image->shapes; shape != NULL; shape = shape->next) {
            NSVGpath* path = shape->paths;
            while (path != NULL) {
                int point_count = path->npts;
                for (int i = 0; i < point_count - 1; i += 3) {
                    float* p = &path->pts[i * 2];
                    gpath->AddBezier(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
                }
                graphics->DrawPath(pen, gpath);  // Draw outline

                path = path->next;
                if (path != NULL)
                {
                    delete reg;
                    reg = gcnew Region(gpath);
                    graphics->ExcludeClip(reg);  // Crop secondary shapes included in outline area
                }
                else
                {
                    graphics->FillPath(Brushes::LightGreen, gpath);  // Fill outline
                }
                gpath->Reset();
            }
        }

        delete pen;
        delete reg;
        delete gpath;
        delete graphics;

        nsvgDelete(image);

        return bmp;
    }

    void CodeRain::paintImageGrid(Bitmap^ image, PictureBox^ codeRainBox, PaintEventArgs^ e)
    {
        float emptySpacePercent = 0.15;
        int columnNumber = 50;
        int width = codeRainBox->Width;
        int height = codeRainBox->Height;

        float spaceColumns = columnNumber * emptySpacePercent;
        float fullGridCellSize = width / (float)columnNumber;
        float spacedGridCellSize = width / (columnNumber + spaceColumns);
        int rowNumber = height / fullGridCellSize;

        float firstColumnBuffer = fullGridCellSize * emptySpacePercent / 2;
        float firstRowBuffer = fullGridCellSize * emptySpacePercent / 4;

        for (int x = columnNumber - 1; x >= 0; x--)
        {
            for (int y = rowNumber; y >= 0; y--)
            {
                float opacity = 1;

                if (x % 2 == 0 && y % 2 == 0)
                    opacity = 0.25;
                else if (x % 2 == 0)
                    opacity = 0.5;
                else if (y % 2 == 0)
                    opacity = 0.75;

                ColorMatrix^ cm = gcnew ColorMatrix();
                cm->Matrix33 = opacity;
                ImageAttributes^ ia = gcnew ImageAttributes();
                ia->SetColorMatrix(cm);
                e->Graphics->DrawImage(image, System::Drawing::Rectangle(firstColumnBuffer + x * fullGridCellSize, firstRowBuffer + y * fullGridCellSize, spacedGridCellSize, spacedGridCellSize),
                    0, 0, image->Width, image->Height, GraphicsUnit::Pixel, ia);

                delete ia;
                delete cm;
            }
        }
    }
}
