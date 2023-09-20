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

        this->netToRaindrop = new NetToRaindropParams();
        this->preparationSuccess = this->netToRaindrop->getSuccess();

        Managed::setDropletOutline(0, Color::FromArgb(15, 27, 100, 43), Color::FromArgb(15, 27, 100, 43));
        Managed::setDropletInner(0, Color::FromArgb(78, 214, 108), Color::FromArgb(78, 214, 108));
        Managed::setDropletFirst(0, Color::FromArgb(224, 255, 206), Color::FromArgb(224, 255, 206));

        Managed::setDropletOutline(1, Color::FromArgb(15, 27, 100, 43), Color::FromArgb(15, 100, 43, 27));
        Managed::setDropletInner(1, Color::FromArgb(138, 214, 108), Color::FromArgb(214, 108, 78));
        Managed::setDropletFirst(1, Color::FromArgb(224, 255, 206), Color::FromArgb(255, 224, 206));
        
        Managed::setImages(this->makeImages(this->vectors));

        this->raindrops = raindrops;

        this->codeCloud[0] = new CodeCloud(raindrops);
        this->codeCloud[1] = new CodeCloud(raindrops);
    }

    CodeRain::~CodeRain()
    {
        Managed::ReleaseResources();

        delete this->resourceHandler;
        delete this->vectors;

        for (int i = 0; i < 2; i++)
        {
            delete this->codeCloud[i];
        }
        delete[] &this->codeCloud;
        delete this->netToRaindrop;
    }

    void CodeRain::Managed::ReleaseResources()
    {
        Managed::ReleaseBitmaps();

        for (int i = 0; i < 2; i++)
        {
            delete Managed::droplet_outline[i];
            delete Managed::droplet_inner[i];
            delete Managed::droplet_first[i];

            delete Managed::droplet_outline_alt[i];
            delete Managed::droplet_inner_alt[i];
            delete Managed::droplet_first_alt[i];
        }

        Managed::droplet_outline->Clear();
        delete Managed::droplet_outline;
        Managed::droplet_inner->Clear();
        delete Managed::droplet_inner;
        Managed::droplet_first->Clear();
        delete Managed::droplet_first;

        Managed::droplet_outline_alt->Clear();
        delete Managed::droplet_outline_alt;
        Managed::droplet_inner_alt->Clear();
        delete Managed::droplet_inner_alt;
        Managed::droplet_first_alt->Clear();
        delete Managed::droplet_first_alt;
    }

    void CodeRain::Managed::ReleaseBitmaps()
    {
        if (Managed::images != nullptr)
        {
            for (int i = 0; i < Managed::images->Count; i++)
            {
                for (int j = 0; j < Managed::images[i]->Count; j++)
                {
                    Managed::ReleaseBitmapAt(Managed::images[i], j);
                }
                Managed::images[i]->Clear();
                delete Managed::images[i];
            }
            Managed::images->Clear();
            delete Managed::images;
        }

        if (Managed::images_alt != nullptr)
        {
            for (int i = 0; i < Managed::images_alt->Count; i++)
            {
                for (int j = 0; j < Managed::images_alt[i]->Count; j++)
                {
                    Managed::ReleaseBitmapAt(Managed::images_alt[i], j);
                }
                Managed::images_alt[i]->Clear();
                delete Managed::images_alt[i];
            }
            Managed::images_alt->Clear();
            delete Managed::images_alt;
        }
    }
    
    void CodeRain::Managed::ReleaseBitmapAt(List<Bitmap^>^ list, int index)
    {
        delete list[index];
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

    List<Bitmap^>^ CodeRain::Managed::getImages(int offs, bool preparationSuccess)
    {
        if (preparationSuccess)
        {
            return Managed::images_alt[offs];
        }
        else
        {
            return Managed::images[offs];
        }
    }

    Color CodeRain::Managed::getDropletOutline(int offs, bool preparationSuccess)
    {
        if (preparationSuccess)
        {
            return Managed::droplet_outline_alt[offs];
        }
        else
        {
            return Managed::droplet_outline[offs];
        }
    }

    Color CodeRain::Managed::getDropletInner(int offs, bool preparationSuccess)
    {
        if (preparationSuccess)
        {
            return Managed::droplet_inner_alt[offs];
        }
        else
        {
            return Managed::droplet_inner[offs];
        }
    }

    Color CodeRain::Managed::getDropletFirst(int offs, bool preparationSuccess)
    {
        if (preparationSuccess)
        {
            return Managed::droplet_first_alt[offs];
        }
        else
        {
            return Managed::droplet_first[offs];
        }
    }

    void CodeRain::Managed::setDropletOutline(int offs, Color color, Color color_alt)
    {
        if (Managed::droplet_outline == nullptr)
        {
            Managed::droplet_outline = gcnew List<Color>();
        }

        if (Managed::droplet_outline->Count == offs)
        {
            Managed::droplet_outline->Add(color);
        }
        else
        {
            delete Managed::droplet_outline[offs];
            Managed::droplet_outline[offs] == color;
        }

        if (Managed::droplet_outline_alt == nullptr)
        {
            Managed::droplet_outline_alt = gcnew List<Color>();
        }

        if (Managed::droplet_outline_alt->Count == offs)
        {
            Managed::droplet_outline_alt->Add(color_alt);
        }
        else
        {
            delete Managed::droplet_outline_alt[offs];
            Managed::droplet_outline_alt[offs] == color_alt;
        }
    }

    void CodeRain::Managed::setDropletInner(int offs, Color color, Color color_alt)
    {
        if (Managed::droplet_inner == nullptr)
        {
            Managed::droplet_inner = gcnew List<Color>();
        }

        if (Managed::droplet_inner->Count == offs)
        {
            Managed::droplet_inner->Add(color);
        }
        else
        {
            delete Managed::droplet_inner[offs];
            Managed::droplet_inner[offs] == color;
        }

        if (Managed::droplet_inner_alt == nullptr)
        {
            Managed::droplet_inner_alt = gcnew List<Color>();
        }

        if (Managed::droplet_inner_alt->Count == offs)
        {
            Managed::droplet_inner_alt->Add(color_alt);
        }
        else
        {
            delete Managed::droplet_inner_alt[offs];
            Managed::droplet_inner_alt[offs] == color_alt;
        }
    }

    void CodeRain::Managed::setDropletFirst(int offs, Color color, Color color_alt)
    {
        if (Managed::droplet_first == nullptr)
        {
            Managed::droplet_first = gcnew List<Color>();
        }

        if (Managed::droplet_first->Count == offs)
        {
            Managed::droplet_first->Add(color);
        }
        else
        {
            delete Managed::droplet_first[offs];
            Managed::droplet_first[offs] == color;
        }

        if (Managed::droplet_first_alt == nullptr)
        {
            Managed::droplet_first_alt = gcnew List<Color>();
        }

        if (Managed::droplet_first_alt->Count == offs)
        {
            Managed::droplet_first_alt->Add(color_alt);
        }
        else
        {
            delete Managed::droplet_first_alt[offs];
            Managed::droplet_first_alt[offs] == color_alt;
        }
    }

    List<List<List<Bitmap^>^>^>^ CodeRain::makeImages(char** vectors)
    {
        List<List<List<Bitmap^>^>^>^ images_alt = gcnew List<List<List<Bitmap^>^>^>();
        Bitmap^ bmp = nullptr;
        bool preparationSuccess = false;
        for (int alt = 0; alt < 2; alt++)
        {
            List<List<Bitmap^>^>^ images = gcnew List<List<Bitmap^>^>();
            for (int i = 0; i < 2; i++)
            {
                List<Bitmap^>^ bmp_list = gcnew List<Bitmap^>();
                int j = 0;
                do
                {
                    bmp = CodeRain::resourceToBitmap(vectors[j], i, preparationSuccess);
                    if (bmp != nullptr)
                    {
                        bmp_list->Add(bmp);
                        j++;
                    }
                } while (vectors[j] != nullptr);
                images->Add(bmp_list);
            }
            images_alt->Add(images);
            preparationSuccess = true;
        }
        return images_alt;
    }

    void CodeRain::Managed::setImages(List<List<List<Bitmap^>^>^>^ images)
    {
        Managed::images = images[0];
        Managed::images_alt = images[1];
    }

    void CodeRain::paint(PictureBox^ codeRainBox, PaintEventArgs^ e)
    {
        this->preparationSuccess = this->netToRaindrop->getSuccess();

        e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;
        e->Graphics->CompositingMode = CompositingMode::SourceCopy;
        e->Graphics->InterpolationMode = InterpolationMode::NearestNeighbor;
        e->Graphics->CompositingQuality = CompositingQuality::HighSpeed;
        
        this->netToRaindrop->RefreshPacketCount();

        CodeRain::paintFromCloud(0, codeRainBox, e);
        CodeRain::paintFromCloud(1, codeRainBox, e);
    }

    Bitmap^ CodeRain::resourceToBitmap(char* res_str, int offs, bool preparationSuccess)
    {
        float scale = 0.09f;

        size_t str_len = strlen(res_str);
        char* res_str_copy = new char[str_len + 1];
        strcpy(res_str_copy, res_str);
        struct NSVGimage* image = nsvgParse(res_str_copy, "px", 96);

        Bitmap^ bmp = gcnew Bitmap(image->width * scale, image->height * scale);
        GraphicsPath^ gpath = gcnew GraphicsPath(FillMode::Winding);
        Region^ reg = gcnew Region();
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

                int steps = 10;
                for (int i = 0; i < steps; i++)
                {
                    Pen^ pen = gcnew Pen(Managed::getDropletOutline(offs, preparationSuccess), i);
                    pen->Alignment = PenAlignment::Outset;
                    graphics->DrawPath(pen, gpath);  // Draw outline
                    delete pen;
                }
        
                path = path->next;
                if (path != NULL)
                {
                    delete reg;
                    reg = gcnew Region(gpath);
                    graphics->ExcludeClip(reg);  // Crop secondary shapes included in outline area
                }
                else
                {
                    Brush^ br = gcnew SolidBrush(Managed::getDropletInner(offs, preparationSuccess));
                    graphics->FillPath(br, gpath);  // Fill outline
                    delete br;
                }
                gpath->Reset();
            }
        }
        
        delete mx;
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
                int destX = firstColumnBuffer + x * fullGridCellSize;
                int destY = firstRowBuffer + (y + droplet_offset) * fullGridCellSize;
                int destWidth = spacedGridCellSize;
                int destHeight = spacedGridCellSize;

                if (destX >= 0 && destY >= 0 && destX <= width && destY <= height)
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
                        cmp->OldColor = Managed::getDropletInner(offs, this->preparationSuccess);
                        cmp->NewColor = Managed::getDropletFirst(offs, this->preparationSuccess);
                        ia->SetGamma(0.3);
                    }

                    cmp_arr = gcnew array<ColorMap^> { cmp };

                    ia->SetRemapTable(cmp_arr);

                    delete cmp_arr;
                    delete cmp;

                    ia->SetColorMatrix(cmx);

                    System::Drawing::Rectangle^ rect = gcnew System::Drawing::Rectangle(destX, destY, destWidth, destHeight);

                    List<Bitmap^>^ imgs = Managed::getImages(offs, this->preparationSuccess);
                    e->Graphics->DrawImage(imgs[symbol], *rect, 0, 0, imgs[symbol]->Width, imgs[symbol]->Height,
                        GraphicsUnit::Pixel, ia);
                    delete imgs;

                    delete rect;
                    delete ia;
                    delete cmx;
                }

                if (i == tail_length - 1 && y + droplet_offset > rowNumber)
                {
                    this->netToRaindrop->CalculateRaindropParams();
                    int length = this->netToRaindrop->getTailLength(offs);
                    this->codeCloud[offs]->reset_raindrop(x, rowNumber, length);
                }

                y++;
            }
            x++;
        } while (raindrops[x] != NULL);
        this->codeCloud[offs]->MakeItRain();
    }
}
