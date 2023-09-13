#include "../pch.h"
#include "CodeRain.h"

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

    CodeRain* CodeRain::instancePtr = NULL;

    CodeRain::CodeRain(int raindrops)
    {
        this->resourceHandler = new N_CodeRain_Res::ResourceHandler();
        this->vectors = resourceHandler->GetAllVectors();

        Managed::droplet_default_outline = gcnew List<Color>();
        Managed::droplet_default_inner = gcnew List<Color>();
        Managed::droplet_inner = gcnew List<Color>();
        Managed::droplet_glow = gcnew List<Color>();
        Managed::droplet_first = gcnew List<Color>();

        Managed::droplet_default_outline->Add(Color::FromArgb(100, 27, 100, 43));
        Managed::droplet_default_inner->Add(Color::FromArgb(78, 214, 108));
        Managed::droplet_inner->Add(Color::FromArgb(37, 204, 79));
        Managed::droplet_glow->Add(Color::FromArgb(215, 255, 192));
        Managed::droplet_first->Add(Color::FromArgb(224, 255, 206));

        Managed::droplet_default_outline->Add(Color::FromArgb(100, 27, 100, 43));
        Managed::droplet_default_inner->Add(Color::FromArgb(138, 214, 108));
        Managed::droplet_inner->Add(Color::FromArgb(87, 204, 79));
        Managed::droplet_glow->Add(Color::FromArgb(215, 255, 192));
        Managed::droplet_first->Add(Color::FromArgb(224, 255, 206));

        Managed::images = gcnew List<List<Bitmap^>^>();
        Bitmap^ bmp = nullptr;
        for (int i = 0; i < 2; i++)
        {
            List<Bitmap^>^ bmp_list = gcnew List<Bitmap^>();
            int j = 0;
            do
            {
                bmp = CodeRain::resourceToBitmap(this->vectors[j], i);
                if (bmp != nullptr)
                {
                    bmp_list->Add(bmp);
                    j++;
                }
            } while (this->vectors[j] != nullptr);
            Managed::images->Add(bmp_list);
        }

        this->raindrops = raindrops;
        this->codeCloud[0] = new CodeCloud(raindrops);
        this->codeCloud[1] = new CodeCloud(raindrops);
    }

    CodeRain::~CodeRain()
    {
        delete this->resourceHandler;
        delete this->vectors;

        for (int i = 0; i < Managed::images->Count; i++)
        {
            for (int j = 0; j < Managed::images[i]->Count; j++)
            {
                delete Managed::images[i][j];
            }
            delete Managed::images[i];
        }
        delete Managed::images;

        for (int i = 0; i < 2; i++)
        {
            delete this->codeCloud[i];

            delete Managed::droplet_default_outline[i];
            delete Managed::droplet_default_inner[i];
            delete Managed::droplet_inner[i];
            delete Managed::droplet_glow[i];
            delete Managed::droplet_first[i];
        }
        delete this->codeCloud;

        delete Managed::droplet_inner;
        delete Managed::droplet_glow;
        delete Managed::droplet_first;
    }

    CodeRain* CodeRain::getInstance() {
        if (instancePtr == NULL)
        {
            instancePtr = new CodeRain(60);
            return instancePtr;
        }
        else
        {
            return instancePtr;
        }
    }

    void CodeRain::paint(PictureBox^ codeRainBox, PaintEventArgs^ e)
    {
        e->Graphics->SmoothingMode = SmoothingMode::HighQuality;
        
        e->Graphics->FillRectangle(Brushes::Black, 0, 0, codeRainBox->Width, codeRainBox->Height);
        
        CodeRain::paintFromCloud(0, codeRainBox, e);
        CodeRain::paintFromCloud(1, codeRainBox, e);
    }

    Bitmap^ CodeRain::resourceToBitmap(char* res_str, int offs)
    {
        float scale = 0.09f;

        size_t str_len = strlen(res_str);
        char* res_str_copy = new char[str_len];
        strcpy(res_str_copy, res_str);
        struct NSVGimage* image = nsvgParse(res_str_copy, "px", 96);
        
        Bitmap^ bmp = gcnew Bitmap(image->width * scale, image->height * scale);
        GraphicsPath^ gpath = gcnew GraphicsPath(FillMode::Winding);
        Region^ reg = gcnew Region();
        Pen^ pen = gcnew Pen(Managed::droplet_default_outline[offs], 3);
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
                    Brush^ br = gcnew SolidBrush(Managed::droplet_default_inner[offs]);
                    graphics->FillPath(br, gpath);  // Fill outline
                    delete br;
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

    void CodeRain::paintFromCloud(int offs, PictureBox^ codeRainBox, PaintEventArgs^ e)
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

        Raindrop** raindrops = this->codeCloud[offs]->inspect_raindrops();
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

                ColorMap^ cmp = gcnew ColorMap();
                array<ColorMap^>^ cmp_arr = nullptr;
                if (i == 0)
                {
                    cmp->OldColor = Managed::droplet_default_inner[offs];
                    cmp->NewColor = Managed::droplet_first[offs];
                    ia->SetGamma(0.3);
                }

                cmp_arr = gcnew array<ColorMap^> { cmp };

                ia->SetRemapTable(cmp_arr);

                delete cmp_arr;
                delete cmp;

                ia->SetColorMatrix(cmx);

                if (i == tail_length - 1 && y + droplet_offset > rowNumber)
                {
                    raindrops[x]->reset_droplet(rowNumber);
                }

                System::Drawing::Rectangle^ rect = gcnew System::Drawing::Rectangle(firstColumnBuffer + x * fullGridCellSize,
                    firstRowBuffer + (y + droplet_offset) * fullGridCellSize,
                    spacedGridCellSize, spacedGridCellSize);

                List<Bitmap^>^ imgs = Managed::images[offs];
                e->Graphics->DrawImage(imgs[symbol], *rect, 0, 0, imgs[symbol]->Width, imgs[symbol]->Height,
                    GraphicsUnit::Pixel, ia);
                delete imgs;

                delete rect;
                delete ia;
                delete cmx;

                y++;
            }
            x++;
        } while (raindrops[x] != NULL);
        this->codeCloud[offs]->MakeItRain();
    }
}
