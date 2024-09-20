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
		bool RenderRedChannel;
		bool RenderGreenChannel;
		bool RenderBlueChannel;

		ToolWindow(void)
		{
			InitializeComponent();

			RenderRedChannel = redCheckBox->Checked;
			RenderGreenChannel = greenCheckBox->Checked;
			RenderBlueChannel = blueCheckBox->Checked;
		}

		void UpdateShaderColor(Shader* shader)
		{
			GLint location = 0;
			location = glGetUniformLocation(shader->GetProgramID(), "RenderRedChannel");
			glUniform1i(location, (int)RenderRedChannel);
			location = glGetUniformLocation(shader->GetProgramID(), "RenderGreenChannel");
			glUniform1i(location, (int)RenderGreenChannel);
			location = glGetUniformLocation(shader->GetProgramID(), "RenderBlueChannel");
			glUniform1i(location, (int)RenderBlueChannel);
		}

	protected:
		virtual ~ToolWindow()
		{
			if (components) delete components;
		}

	private: System::Windows::Forms::CheckBox^ redCheckBox;
	private: System::Windows::Forms::CheckBox^ greenCheckBox;
	private: System::Windows::Forms::CheckBox^ blueCheckBox;

	private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary> Required method for Designer support - do not modify
		/// the contents of this method with the code editor.</summary>
		void InitializeComponent(void)
		{
			this->redCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->greenCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->blueCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// redCheckBox
			// 
			this->redCheckBox->AutoSize = true;
			this->redCheckBox->Location = System::Drawing::Point(12, 12);
			this->redCheckBox->Name = L"redCheckBox";
			this->redCheckBox->Size = System::Drawing::Size(88, 17);
			this->redCheckBox->TabIndex = 0;
			this->redCheckBox->Text = L"Red Channel";
			this->redCheckBox->UseVisualStyleBackColor = true;
			this->redCheckBox->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::redCheckBox_CheckedChanged);
			// 
			// greenCheckBox
			// 
			this->greenCheckBox->AutoSize = true;
			this->greenCheckBox->Location = System::Drawing::Point(12, 35);
			this->greenCheckBox->Name = L"greenCheckBox";
			this->greenCheckBox->Size = System::Drawing::Size(97, 17);
			this->greenCheckBox->TabIndex = 1;
			this->greenCheckBox->Text = L"Green Channel";
			this->greenCheckBox->UseVisualStyleBackColor = true;
			this->greenCheckBox->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::greenCheckBox_CheckedChanged);
			// 
			// blueCheckBox
			// 
			this->blueCheckBox->AutoSize = true;
			this->blueCheckBox->Location = System::Drawing::Point(12, 58);
			this->blueCheckBox->Name = L"blueCheckBox";
			this->blueCheckBox->Size = System::Drawing::Size(89, 17);
			this->blueCheckBox->TabIndex = 2;
			this->blueCheckBox->Text = L"Blue Channel";
			this->blueCheckBox->UseVisualStyleBackColor = true;
			this->blueCheckBox->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::blueCheckBox_CheckedChanged);
			// 
			// ToolWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(212, 206);
			this->Controls->Add(this->blueCheckBox);
			this->Controls->Add(this->greenCheckBox);
			this->Controls->Add(this->redCheckBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"ToolWindow";
			this->Text = L"Color Tool";
			this->TopMost = true;
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void redCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
	{
		RenderRedChannel = redCheckBox->Checked;
	}

	private: System::Void greenCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
	{
		RenderGreenChannel = greenCheckBox->Checked;
	}

	private: System::Void blueCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
	{
		RenderBlueChannel = blueCheckBox->Checked;
	}
	};
}