// Copyright Richard Kaiser, 2022. https://www.rkaiser.de. All rights reserved.
// It is permitted to use this source text in programs of your own, provided that 
// it contains a reference to the source (the first line above). All 
// liabilities for use of the code are disclaimed.

#ifndef CPPCLR_UTILS_H
#define CPPCLR_UTILS_H

#if !__cplusplus_cli 
#error Only for C++/CLI Applications (e.g. Visual Studio Windows Forms Projects)
#endif

#include <string>
#define SIMPLE_STRING_CONVERSIONS 1
#if SIMPLE_STRING_CONVERSIONS

namespace rk1 {
    /*
    Conversion functions for .NET strings in C++ Windows Forms projects
    (Visual Studio 2008 and later). These functions can only be used
    in a CLR Windows Forms project.
    */
    std::string to_string(System::String^ S)
    {
        std::string s;
        for (int i = 0; i < S->Length; i++)
            s += S[i];
        return s;
    }

    System::String^ to_String(std::string s)
    {
        return gcnew System::String(s.c_str());
    }

    // Overloaded variants for char* and wstring

    char* toCharPtr(System::String^ S)
    {
        using System::Runtime::InteropServices::Marshal;
        char* p = (char*)Marshal::StringToHGlobalAnsi(S).ToPointer();

        //// Always free the unmanaged string.
        //Marshal::FreeHGlobal(IntPtr(stringPointer));
        return p;
    }

    System::String^ to_String(const char* s)
    {
        return gcnew System::String(s);
    }

    System::String^ to_String(const wchar_t* s)
    {
        return gcnew System::String(s);
    }

    System::String^ to_String(std::wstring s)
    {
        return gcnew System::String(s.c_str());
    }


} // end of namespace rk1 

#else // #if SIMPLE_STRING_CONVERSIONS 

#include <cstdio>

#include <string>
#include <msclr\marshal_cppstd.h>

namespace rk1 {

    // using System::String;
    // These marshal_as functions can be used to convert other data types
    // used: http://msdn.microsoft.com/en-us/library/bb384865.aspx

    // The #include statement for this file should appear before any other using statements for
    // .Net appear (e.g. using System;). Otherwise, a barrage of error messages will
    // be the result. Therefore it is best to include it at the beginning of MatrixForm.h.

#pragma message ("CppClr_Utils.h must be #included before all other .Net usings, ")
#pragma message ("e.g. before 'using namespace System'. Otherwise there may lots of")
#pragma message ("compiler errors like")
#pragma message ("IDataObject: Mehrdeutiges Symbol")

    using System::Windows::Forms::TextBox;

    System::String^ to_String(std::string s)
    {
        return msclr::interop::marshal_as<System::String^>(s);
    }

    /*
    System::String^ to_String(const std::string& s)
    {
    return msclr::interop::marshal_as<System::String^>(s);
    }
    */

    /* Possible Alternative
    System::String^ to_String(std::string s)
    {
    return gcnew System::String(s.c_str());
    }
    */

    std::string to_string(System::String^ S)
    {
        return msclr::interop::marshal_as<std::string>(S);
    }

    /* Possible Alternative

    std::string to_string(System::String^ S)
    {
    std::string s;
    for (int i=0;i<S->Length; i++)
    s+=S[i];
    return s;
    }
    */

    System::String^ to_String(std::wstring s)
    {
        return msclr::interop::marshal_as<System::String^>(s);
    }
    /*
    System::String^ to_String(const std::wstring& s)
    {
    return msclr::interop::marshal_as<System::String^>(s);
    }
    */
    std::wstring to_wstring(System::String^ S)
    {
        return msclr::interop::marshal_as<std::wstring>(S);
    }

    void __test_converions()
    {
        // These statements will need to be compiled
        // with the conversion functions above:
        using System::String; // only works on C++ in Windows
        // Forms application with Visual Studio
        using std::string;
        char* c1 = "9ou9ouo";
        const char* c2 = "9ou9ouo";
        String^ SC1 = rk1::to_String(c1);
        String^ SC2 = rk1::to_String(c2);
        String^ SC3 = rk1::to_String("jjj");

        string s1 = "9ou9ouo";
        const string s2 = "9ou9ouo";
        String^ S1 = rk1::to_String(s1);
        String^ S2 = rk1::to_String(s2);
        String^ S3 = rk1::to_String("jojopj");

        string r1 = rk1::to_string(S1);
        string r2 = rk1::to_string(S2);
        string r3 = rk1::to_string("ikhhi");
    }
}
#endif // #if SIMPLE_STRING_CONVERSIONS 

namespace rk1 {
    // using namespace System;
    // using namespace System::Windows::Forms;

    ref class StaticTextBox // "ref" defines a .NET class.
    { // Allows output to a TextBox without having to pass it as a
      // parameters.
      //
      // This TextBox must be set before the first call of WriteLine by a
      // call of Init, e.g. in the constructor of the form.

        static System::Windows::Forms::TextBox^ tb = nullptr;
    public:
        static void Init(System::Windows::Forms::TextBox^ tb_) { tb = tb_; }

        static void WriteLine(std::string msg)
        {
            if (tb != nullptr)
                tb->AppendText(System::String::Format(rk1::to_String(msg) + "\r\n"));
            else
                System::Windows::Forms::MessageBox::Show(
                    "StaticTextBox::texBox == nullptr (Init vergessen)");
        }

        /* The following functions can also be useful for this class:

        static void WriteLine(const char* msg)
        {
        if (tb != nullptr)
          tb->AppendText(String::Format(rk1::to_String(msg) + "\r\n"));
        }

        static void WriteLine(String^ format)
        {
          if (tb != nullptr)
            tb->AppendText(format + "\r\n");
          else
            MessageBox::Show("StaticTextBox::texBox == nullptr (Init vergessen)");
        }

        static void WriteLine(String^ format, Object^ arg0)
        {
          if (tb != nullptr)
            tb->AppendText(String::Format(format + "\r\n", arg0));
          else
            MessageBox::Show("StaticTextBox::texBox == nullptr (Init vergessen)");
        }

        static void WriteLine(String^ format, Object^ arg0, Object^ arg1)
        {
          if (tb != nullptr)
            tb->AppendText(String::Format(format + "\r\n", arg0, arg1));
          else
            MessageBox::Show("StaticTextBox::texBox == nullptr (Init vergessen)");
        }

        static void WriteLine(String^ format, Object^ arg0, Object^ arg1, Object^ arg2)
        {
          if (tb != nullptr)
            tb->AppendText(String::Format(format + "\r\n", arg0, arg1, arg2));
          else
            MessageBox::Show("StaticTextBox::texBox == nullptr (Init vergessen)");
        }
        */
    };

    void test_StaticTextBox(System::Windows::Forms::TextBox^ tb)
    {
        rk1::StaticTextBox::Init(tb);
        // StaticTextBox can be used similarly to System::Console:
        // using namespace System::Console;
        /*
        System::Console::WriteLine("Hallo");
        System::Console::WriteLine("Hallo 1={0}",1);
        System::Console::WriteLine("Hallo 1={0} 2={1}",1,2);
        System::Console::WriteLine("Hallo  1={0} 2={1} 3={2}",1,2,3);
        */
        rk1::StaticTextBox::WriteLine("Hallo");
        /*
        rk1::StaticTextBox::WriteLine("Hallo 1={0}", 1);
        rk1::StaticTextBox::WriteLine("Hallo 1={0} 2={1}", 1, 2);
        rk1::StaticTextBox::WriteLine("Hallo 1={0} 2={1} 3={2}", 1, 2, 3);
        */
    }
} // end of namespace rk1

#endif // !CPPCLR_UTILS_H
