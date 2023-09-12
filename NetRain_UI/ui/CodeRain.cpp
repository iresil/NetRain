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

        Managed::droplet_default_inner = Color::White;
        Managed::droplet_default_outline = Color::Gray;

        Managed::droplet_inner = gcnew List<Color>();
        Managed::droplet_glow = gcnew List<Color>();
        Managed::droplet_first = gcnew List<Color>();

        Managed::droplet_inner->Add(Color::LightGreen);
        Managed::droplet_glow->Add(Color::FromArgb(32, 155, 63));
        Managed::droplet_first->Add(Color::FromArgb(248, 255, 216));

        Managed::droplet_inner->Add(Color::Orange);
        Managed::droplet_glow->Add(Color::FromArgb(155, 63, 32));
        Managed::droplet_first->Add(Color::FromArgb(255, 248, 216));

        List<Bitmap^>^ bmp_list = gcnew List<Bitmap^>();
        Bitmap^ bmp = nullptr;
        int i = 0;
        do
        {
            bmp = CodeRain::resourceToBitmap(this->vectors[i]);
            if (bmp != nullptr)
            {
                bmp_list->Add(bmp);
                i++;
            }
        } while (this->vectors[i] != nullptr);
        Managed::images = bmp_list;

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
            delete Managed::images[i];
        }
        delete Managed::images;

        for (int i = 0; i < 2; i++)
        {
            delete this->codeCloud[i];

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
        
        CodeRain::paintFromCloud(this->codeCloud, 0, Managed::images, codeRainBox, e);
        CodeRain::paintFromCloud(this->codeCloud, 1, Managed::images, codeRainBox, e);
    }

    Bitmap^ CodeRain::resourceToBitmap(char* res_str)
    {
        float scale = 0.09f;

        size_t str_len = strlen(res_str);
        char* res_str_copy = new char[str_len];
        strcpy(res_str_copy, res_str);
        struct NSVGimage* image = nsvgParse(res_str_copy, "px", 96);
        
        Bitmap^ bmp = gcnew Bitmap(image->width * scale, image->height * scale);
        GraphicsPath^ gpath = gcnew GraphicsPath(FillMode::Winding);
        Region^ reg = gcnew Region();
        Pen^ pen = gcnew Pen(*Managed::droplet_default_outline, 3);
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
                    Brush^ br = gcnew SolidBrush(*Managed::droplet_default_inner);
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

    void CodeRain::paintFromCloud(CodeCloud** codeCloud, int offs, List<Bitmap^>^ images, PictureBox^ codeRainBox, PaintEventArgs^ e)
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

        Raindrop** raindrops = codeCloud[offs]->inspect_raindrops();
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
                    cmp->OldColor = *Managed::droplet_default_inner;
                    cmp->NewColor = Managed::droplet_first[offs];
                }
                else
                {
                    cmp->OldColor = *Managed::droplet_default_inner;
                    cmp->NewColor = Managed::droplet_inner[offs];
                }

                ColorMap^ cmp_border = gcnew ColorMap();
                cmp_border->OldColor = *Managed::droplet_default_outline;
                cmp_border->NewColor = Managed::droplet_glow[offs];

                cmp_arr = gcnew array<ColorMap^> { cmp, cmp_border };

                ia->SetRemapTable(cmp_arr);

                delete cmp_arr;
                delete cmp_border;
                delete cmp;

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
        codeCloud[offs]->MakeItRain();
    }
}
