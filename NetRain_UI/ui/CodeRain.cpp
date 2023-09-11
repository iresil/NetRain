#include "../pch.h"
#include "CodeRain.h"
#include "../../NetRain_Resources/ResourceHandler.h"

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

    CodeRain::CodeRain(int raindrops)
    {
        this->raindrops = raindrops;
        this->codeCloud = new CodeCloud(raindrops);
    }

    CodeRain::~CodeRain()
    {
        delete this->codeCloud;
    }

    CodeRain& CodeRain::getInstance() {
        static CodeRain& instance = *(new CodeRain(60));
        return instance;
    }

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
        CodeRain::paintFromCloud(bmp_list, codeRainBox, e);

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
        float scale = 0.09f;

        struct NSVGimage* image = nsvgParse(res_str_nterm, "px", 96);

        Bitmap^ bmp = gcnew Bitmap(image->width * scale, image->height * scale);
        GraphicsPath^ gpath = gcnew GraphicsPath(FillMode::Winding);
        Region^ reg = gcnew Region();
        Pen^ pen = gcnew Pen(Color::FromArgb(32, 155, 63), 3);
        Graphics^ graphics = Graphics::FromImage(bmp);
        Matrix^ mx = gcnew Matrix(1.0f / 1.2f, 0, 0, 1.0f / 1.2f, -(1.0f / 1.2f), -(1.0f / 1.2f));

        for (NSVGshape* shape = image->shapes; shape != NULL; shape = shape->next) {
            NSVGpath* path = shape->paths;
            while (path != NULL) {
                int point_count = path->npts;
                for (int i = 0; i < point_count - 1; i += 3) {
                    float* p = &path->pts[i * 2];
                    gpath->AddBezier(p[0] * scale, p[1] * scale, p[2] * scale, p[3] * scale, p[4] * scale, p[5] * scale, p[6] * scale, p[7] * scale);
                }
                graphics->SmoothingMode = SmoothingMode::AntiAlias;
                graphics->Transform = mx;
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

        delete mx;
        delete pen;
        delete reg;
        delete gpath;
        delete graphics;

        nsvgDelete(image);

        return bmp;
    }

    void CodeRain::paintFromCloud(List<Bitmap^>^ images, PictureBox^ codeRainBox, PaintEventArgs^ e)
    {
        float emptySpacePercent = 0.15;
        int width = codeRainBox->Width;
        int height = codeRainBox->Height;

        float spaceColumns = this->raindrops * emptySpacePercent;
        float fullGridCellSize = width / (float)this->raindrops;
        float spacedGridCellSize = width / (this->raindrops + spaceColumns);
        int rowNumber = height / fullGridCellSize;

        float firstColumnBuffer = fullGridCellSize * emptySpacePercent / 2;
        float firstRowBuffer = fullGridCellSize * emptySpacePercent / 4;

        Raindrop** raindrops = this->codeCloud->inspect_raindrops();
        int x = 0;
        do
        {
            int tail_length = raindrops[x]->get_tail_length();
            int droplet_offset = raindrops[x]->get_droplet_offset();

            Droplet** droplets = raindrops[x]->inspect_droplets();
            int y = 0;
            for (int i = tail_length - 1; i >= 0; i--)
            {
                int symbol = droplets[i]->get_next_symbol(raindrops[x]->get_fall_seconds_multiplier());
                float opacity = droplets[i]->get_opacity();

                ColorMatrix^ cmx = gcnew ColorMatrix();
                cmx->Matrix33 = opacity;
                ImageAttributes^ ia = gcnew ImageAttributes();
                if (i == 0)
                {
                    ColorMap^ cmp = gcnew ColorMap();
                    cmp->OldColor = Color::LightGreen;
                    cmp->NewColor = Color::FromArgb(248, 255, 216);
                    array<ColorMap^>^ cmp_arr = gcnew array<ColorMap^> { cmp };

                    ia->SetRemapTable(cmp_arr);

                    delete cmp_arr;
                    delete cmp;
                }
                ia->SetColorMatrix(cmx);

                if (i == tail_length - 1 && y + droplet_offset > rowNumber)
                {
                    raindrops[x]->reset_droplet(rowNumber);
                }

                System::Drawing::Rectangle^ rect = gcnew System::Drawing::Rectangle(firstColumnBuffer + x * fullGridCellSize,
                    firstRowBuffer + (y + droplet_offset) * fullGridCellSize,
                    spacedGridCellSize, spacedGridCellSize);

                e->Graphics->DrawImage(images[symbol], *rect, 0, 0, images[symbol]->Width, images[symbol]->Height,
                    GraphicsUnit::Pixel, ia);

                delete rect;
                delete ia;
                delete cmx;

                y++;
            }
            x++;
        } while (raindrops[x] != NULL);
        this->codeCloud->MakeItRain();
    }
}
