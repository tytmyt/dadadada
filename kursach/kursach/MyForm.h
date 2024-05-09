#include <vector>
#include <algorithm>
#include <cmath>
#include <thread>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class MainForm : public System::Windows::Forms::Form {
private:
    TextBox^ inputDataTextBox;
    Button^ calculateButton;
    Label^ resultLabel;
    Panel^ histogramPanel;

public:
    MainForm() {
        // Налаштування форми
        this->Text = "Статистичний аналіз даних";
        this->Size = Drawing::Size(400, 400);

        // Текстове поле для введення даних
        inputDataTextBox = gcnew TextBox();
        inputDataTextBox->Location = Drawing::Point(20, 20);
        inputDataTextBox->Width = 250;
        this->Controls->Add(inputDataTextBox);

        // Кнопка для обчислення
        calculateButton = gcnew Button();
        calculateButton->Text = "Обчислити та побудувати гістограму";
        calculateButton->Location = Drawing::Point(20, 50);
        calculateButton->Click += gcnew EventHandler(this, &MainForm::OnCalculateButtonClick);
        this->Controls->Add(calculateButton);

        // Етикетка для відображення результатів
        resultLabel = gcnew Label();
        resultLabel->Location = Drawing::Point(20, 80);
        resultLabel->AutoSize = true;
        this->Controls->Add(resultLabel);

        // Панель для гістограми
        histogramPanel = gcnew Panel();
        histogramPanel->Location = Drawing::Point(20, 120);
        histogramPanel->Size = Drawing::Size(350, 250);
        this->Controls->Add(histogramPanel);
    }

    // Обробник події натискання кнопки обчислення
    void OnCalculateButtonClick(Object^ sender, EventArgs^ e) {
        String^ inputData = gcnew String(inputDataTextBox->Text);
        List<double>^ data = ParseData(inputData);

        // Обчислення статистичних показників
        double median = CalculateMedian(data);
        double stdDeviation = CalculateStandardDeviation(data);
        double mode = CalculateMode(data);

        // Побудова гістограми
        DrawHistogram(data, histogramPanel);

        // Відображення результатів
        resultLabel->Text = "Медіана: " + median + "\n";
        resultLabel->Text += "Стандартне відхилення: " + stdDeviation + "\n";
        resultLabel->Text += "Мода: " + mode;
    }

private:
    List<double>^ ParseData(String^ inputData) {
        List<double>^ data = gcnew List<double>();
        array<String^>^ values = inputData->Split(',');
        for each (String ^ value in values) {
            data->Add(Double::Parse(value));
        }
        return data;
    }

    double CalculateMedian(List<double>^ data) {
        List<double>^ sortedData = gcnew List<double>(data);
        sortedData->Sort();
        int n = sortedData->Count;

        if (n % 2 == 0) {
            return (sortedData[n / 2 - 1] + sortedData[n / 2]) / 2.0;
        }
        else {
            return sortedData[n / 2];
        }
}

    double CalculateStandardDeviation(List<double>^ data) {
        double mean = 0.0;
        double variance = 0.0;

        for each (double value in data) {
            mean += value;
        }
        mean /= data->Count;

        for each (double value in data) {
            variance += (value - mean) * (value - mean);
        }
        variance /= data->Count;

        return sqrt(variance);
    }

    double CalculateMode(List<double>^ data) {
        Dictionary<double, int>^ counts = gcnew Dictionary<double, int>();

        for each (double value in data) {
            if (counts->ContainsKey(value)) {
                counts[value]++;
            }
            else {
                counts[value] = 1;
            }
        }

        double mode = 0.0;
        int maxCount = 0;

        for each (KeyValuePair<double, int> ^ pair in counts) {
            if (pair->Value > maxCount) {
                mode = pair->Key;
                maxCount = pair->Value;
            }
        }

        return mode;
    }

    void DrawHistogram(List<double>^ data, Panel^ panel) {
        // Calculate the maximum value in the data array
        double maxValue = data[0];
        for (Int32 i = 1; i < data->Count; i++) {
            if (data[i] > maxValue) {
                maxValue = data[i];
            }
        }

        // Create a Bitmap for the Panel
        Bitmap^ bitmap = gcnew Bitmap(panel->Width, panel->Height);

        // Create a Graphics object for the Bitmap
        Graphics^ graphics = Graphics::FromImage(bitmap);

        // Clear the background
        graphics->Clear(panel->BackColor);

        // Define the pen for drawing the bars
        Pen^ pen = gcnew Pen(Color::Black, 2);

        // Calculate the width and height of each bar
        Int32 barWidth = panel->Width / data->Count;
        Int32 barHeight;

        // Draw the bars
        for (Int32 i = 0; i < data->Count; i++) {
            barHeight = (data[i] * panel->Height) / maxValue;
            graphics->FillRectangle(Brushes::Blue, i * barWidth, panel->Height - barHeight, barWidth, barHeight);
            graphics->DrawRectangle(pen, i * barWidth, panel->Height - barHeight, barWidth, barHeight);
        }

        // Draw the histogram on the Panel
        panel->BackgroundImage = bitmap;
    }

};