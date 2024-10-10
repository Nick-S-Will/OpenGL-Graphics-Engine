#pragma once

namespace OpenGL
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class ToolWindow : public System::Windows::Forms::Form
	{
	public:
		float specularStrength;

		ToolWindow(void)
		{
			InitializeComponent();

			specularStrength = (float)specularStrengthSlider->Value / specularStrengthSlider->Maximum;
		}

	protected:
		virtual ~ToolWindow()
		{
			if (components) delete components;
		}
	private: System::Windows::Forms::TrackBar^ specularStrengthSlider;
	protected:


	private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		   /// <summary> Required method for Designer support - do not modify
		   /// the contents of this method with the code editor.</summary>
		   void InitializeComponent(void)
		   {
			   this->specularStrengthSlider = (gcnew System::Windows::Forms::TrackBar());
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->specularStrengthSlider))->BeginInit();
			   this->SuspendLayout();
			   // 
			   // specularStrengthSlider
			   // 
			   this->specularStrengthSlider->Location = System::Drawing::Point(13, 13);
			   this->specularStrengthSlider->Maximum = 100;
			   this->specularStrengthSlider->Name = L"specularStrengthSlider";
			   this->specularStrengthSlider->Size = System::Drawing::Size(258, 56);
			   this->specularStrengthSlider->TabIndex = 0;
			   this->specularStrengthSlider->TickFrequency = 10;
			   this->specularStrengthSlider->Scroll += gcnew System::EventHandler(this, &ToolWindow::specularStrengthSlider_Scroll);
			   // 
			   // ToolWindow
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(283, 254);
			   this->Controls->Add(this->specularStrengthSlider);
			   this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			   this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			   this->Name = L"ToolWindow";
			   this->Text = L"Color Tool";
			   this->TopMost = true;
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->specularStrengthSlider))->EndInit();
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion

	private:
	System::Void specularStrengthSlider_Scroll(System::Object^ sender, System::EventArgs^ e)
	{
		specularStrength = (float)specularStrengthSlider->Value / specularStrengthSlider->Maximum;
	}

	};
}