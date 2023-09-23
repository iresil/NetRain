#include "pch.h"
#include "ui/MatrixForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ args)
{
    for (int i = 0; i < args->Length; i++)
    {
        String^ cleanArg = args[i]->Trim('\\')->Trim('=')->Trim('/');
        if (cleanArg == "c")
        {
            System::Windows::Forms::MessageBox::Show("No Settings Available");
            return 0;
        }
        else if (cleanArg == "p")
            return 0;
    }

    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew NetRain::MatrixForm());
    return 0;
}
