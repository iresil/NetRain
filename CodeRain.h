#pragma once

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
    using namespace System::Drawing::Drawing2D;
    using namespace System::Collections::Generic;

    void paint(PictureBox^ codeRainBox, PaintEventArgs^ e)
    {
        e->Graphics->SmoothingMode = SmoothingMode::HighQuality;

        GraphicsPath^ gpath = gcnew GraphicsPath(FillMode::Winding);
        Region^ reg = gcnew Region();
        Pen^ pen = gcnew Pen(Brushes::Black);

        const char* filepath = "resources/54.svg";
        struct NSVGimage* image = nsvgParseFromFile(filepath, "px", 96);
        for (NSVGshape* shape = image->shapes; shape != NULL; shape = shape->next) {
            NSVGpath* path = shape->paths;
            while (path != NULL) {
                int point_count = path->npts;
                for (int i = 0; i < point_count - 1; i += 3) {
                    float* p = &path->pts[i * 2];
                    gpath->AddBezier(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
                }
                e->Graphics->DrawPath(pen, gpath);  // Draw outline
                
                path = path->next;
                if (path != NULL)
                {
                    delete reg;
                    reg = gcnew Region(gpath);
                    e->Graphics->ExcludeClip(reg);  // Crop secondary shapes included in outline area
                }
                else
                {
                    e->Graphics->FillPath(Brushes::Black, gpath);  // Fill outline
                }
                gpath->Reset();
            }
        }
        delete pen;
        delete reg;
        delete gpath;

        nsvgDelete(image);
    }
}
