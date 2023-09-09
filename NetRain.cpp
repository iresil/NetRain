#include "pch.h"
#include "ui\MatrixForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main()
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew NetRain::MatrixForm());
    return 0;
}
