#include "../pch.h"
#include "CodeRain.h"
#include "../../NetRain_Resources/ResourceHandler.h"
#include "../../NetRain_CodeWeather/CodeCloud.h"

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

        N_CodeRain_Res::ResourceHandler res_h;
        char** vectors = res_h.GetAllVectors();

        List<Bitmap^>^ bmp_list = gcnew List<Bitmap^>();
        Bitmap^ bmp = nullptr;
        int i = 0;
        do
        {
            bmp = CodeRain::resourceToBitmap(vectors[i]);
            if (bmp != nullptr)
            {
                bmp_list->Add(bmp);
                i++;
            }
        } while (vectors[i] != nullptr);
        CodeRain::paintImageGrid(bmp_list, codeRainBox, e);

        delete bmp;
        for (int i = 0; i < bmp_list->Count; i++)
        {
            delete bmp_list[i];
        }
        delete bmp_list;
        delete vectors;
    }

    Bitmap^ CodeRain::resourceToBitmap(char* res_str)
    {
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

    void CodeRain::paintImageGrid(List<Bitmap^>^ images, PictureBox^ codeRainBox, PaintEventArgs^ e)
    {
        Random^ rand = gcnew Random();
        int columnNumber = 50;
        //CodeCloud* cloud = new CodeCloud(columnNumber);
        //Raindrop** raindrops = cloud->inspect_raindrops();
        //Droplet** droplets = raindrops[0]->inspect_droplets();

        float emptySpacePercent = 0.15;
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

                int random = rand->Next(0, images->Count);

                e->Graphics->DrawImage(images[random], System::Drawing::Rectangle(firstColumnBuffer + x * fullGridCellSize, firstRowBuffer + y * fullGridCellSize, spacedGridCellSize, spacedGridCellSize),
                    0, 0, images[random]->Width, images[random]->Height, GraphicsUnit::Pixel, ia);

                delete ia;
                delete cm;
            }
        }
    }
}
