#pragma once

#include "CodeRain.h"

namespace NetRain
{
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// This is the main form, on which the code rain effect will be displayed.
    /// </summary>
    public ref class MatrixForm : public System::Windows::Forms::Form
    {
    public:
        MatrixForm(void)
        {
            InitializeComponent();
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~MatrixForm()
        {
            if (components)
            {
                delete components;
            }
        }
    private:
        System::Windows::Forms::PictureBox^ codeRainBox;
        System::Windows::Forms::Timer^ timerRefresh;

        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            this->codeRainBox = (gcnew System::Windows::Forms::PictureBox());
            this->timerRefresh = (gcnew System::Windows::Forms::Timer(this->components));
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->codeRainBox))->BeginInit();
            this->SuspendLayout();
            // 
            // codeRainBox
            // 
            this->codeRainBox->BackColor = System::Drawing::Color::Black;
            this->codeRainBox->Dock = System::Windows::Forms::DockStyle::Fill;
            this->codeRainBox->Location = System::Drawing::Point(0, 0);
            this->codeRainBox->Name = L"codeRainBox";
            this->codeRainBox->Size = System::Drawing::Size(723, 589);
            this->codeRainBox->TabIndex = 4;
            this->codeRainBox->TabStop = false;
            this->codeRainBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MatrixForm::codeRainBox_Paint);
            // 
            // timerRefresh
            // 
            this->timerRefresh->Enabled = true;
            this->timerRefresh->Interval = 50;
            this->timerRefresh->Tick += gcnew System::EventHandler(this, &MatrixForm::timerRefresh_Tick);
            // 
            // MatrixForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::Black;
            this->ClientSize = System::Drawing::Size(723, 589);
            this->Controls->Add(this->codeRainBox);
            this->Name = L"MatrixForm";
            this->Text = L"MatrixForm";
            this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->codeRainBox))->EndInit();
            this->ResumeLayout(false);

        }
#pragma endregion

        System::Void codeRainBox_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
        {
            N_CodeRain::CodeRain::getInstance()->paint(codeRainBox, e);
        }

        System::Void timerRefresh_Tick(System::Object^ sender, System::EventArgs^ e)
        {
            codeRainBox->Invalidate();
        }
    }; // end of class MatrixForm
} // end of namespace NetRain
