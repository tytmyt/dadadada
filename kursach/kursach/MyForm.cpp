#include "MyForm.h"
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // ������ �����
    Application::Run(gcnew MainForm());

    return 0;
}
