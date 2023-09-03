#include "pch.h"

using namespace System;

#include "MatrixForm.h"

using namespace System::Windows::Forms;

[STAThread]
int main()
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew CppCLRWinFormsProject::MatrixForm());
    return 0;
}
