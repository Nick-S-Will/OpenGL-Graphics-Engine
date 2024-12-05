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

			UpdateSpecularValues();
		}

	protected:
		virtual ~ToolWindow()
		{
			if (components) delete components;
		}

	private: System::Windows::Forms::RadioButton^ moveLightRadioButton;
	private: System::Windows::Forms::Button^ resetLightPositionButton;
	private: System::Windows::Forms::Label^ specularLabel;
	private: System::Windows::Forms::TrackBar^ specularStrengthTrackBar;
	private: System::Windows::Forms::Label^ specularStrengthValueLabel;

	private: System::Windows::Forms::TrackBar^ rTrackBar;
	private: System::Windows::Forms::Label^ rValueLabel;

	private: System::Windows::Forms::TrackBar^ gTrackBar;
	private: System::Windows::Forms::Label^ gValueLabel;

	private: System::Windows::Forms::TrackBar^ bTrackBar;
	private: System::Windows::Forms::Label^ bValueLabel;
	private: System::Windows::Forms::RadioButton^ transformRadioButton;
	private: System::Windows::Forms::Button^ resetTransformButton;
	private: System::Windows::Forms::RadioButton^ waterSceneRadioButton;

	private: System::Windows::Forms::Label^ rLabel;
	private: System::Windows::Forms::Label^ gLabel;
	private: System::Windows::Forms::Label^ bLabel;

	private: System::Windows::Forms::CheckBox^ translateCheckBox;
	private: System::Windows::Forms::CheckBox^ rotateCheckBox;
	private: System::Windows::Forms::CheckBox^ scaleCheckBox;
	private: System::Windows::Forms::Label^ frequencyLabel;
	private: System::Windows::Forms::TrackBar^ frquencyTrackBar;
	private: System::Windows::Forms::Label^ frequencyValueLabel;
	private: System::Windows::Forms::Label^ amplitudeValueLabel;
	private: System::Windows::Forms::TrackBar^ amplitudeTrackBar;
	private: System::Windows::Forms::Label^ amplitudeLabel;
	private: System::Windows::Forms::CheckBox^ tintBlueCheckBox;
	private: System::Windows::Forms::CheckBox^ wireframeRenderCheckBox;
	private: System::Windows::Forms::RadioButton^ spaceSceneRadioButton;
	private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		   /// <summary> Required method for Designer support - do not modify
		   /// the contents of this method with the code editor.</summary>
		   void InitializeComponent(void)
		   {
			   this->moveLightRadioButton = (gcnew System::Windows::Forms::RadioButton());
			   this->resetLightPositionButton = (gcnew System::Windows::Forms::Button());
			   this->specularLabel = (gcnew System::Windows::Forms::Label());
			   this->specularStrengthTrackBar = (gcnew System::Windows::Forms::TrackBar());
			   this->specularStrengthValueLabel = (gcnew System::Windows::Forms::Label());
			   this->rTrackBar = (gcnew System::Windows::Forms::TrackBar());
			   this->rValueLabel = (gcnew System::Windows::Forms::Label());
			   this->gTrackBar = (gcnew System::Windows::Forms::TrackBar());
			   this->gValueLabel = (gcnew System::Windows::Forms::Label());
			   this->bTrackBar = (gcnew System::Windows::Forms::TrackBar());
			   this->bValueLabel = (gcnew System::Windows::Forms::Label());
			   this->transformRadioButton = (gcnew System::Windows::Forms::RadioButton());
			   this->resetTransformButton = (gcnew System::Windows::Forms::Button());
			   this->waterSceneRadioButton = (gcnew System::Windows::Forms::RadioButton());
			   this->rLabel = (gcnew System::Windows::Forms::Label());
			   this->gLabel = (gcnew System::Windows::Forms::Label());
			   this->bLabel = (gcnew System::Windows::Forms::Label());
			   this->translateCheckBox = (gcnew System::Windows::Forms::CheckBox());
			   this->rotateCheckBox = (gcnew System::Windows::Forms::CheckBox());
			   this->scaleCheckBox = (gcnew System::Windows::Forms::CheckBox());
			   this->frequencyLabel = (gcnew System::Windows::Forms::Label());
			   this->frquencyTrackBar = (gcnew System::Windows::Forms::TrackBar());
			   this->frequencyValueLabel = (gcnew System::Windows::Forms::Label());
			   this->amplitudeValueLabel = (gcnew System::Windows::Forms::Label());
			   this->amplitudeTrackBar = (gcnew System::Windows::Forms::TrackBar());
			   this->amplitudeLabel = (gcnew System::Windows::Forms::Label());
			   this->tintBlueCheckBox = (gcnew System::Windows::Forms::CheckBox());
			   this->wireframeRenderCheckBox = (gcnew System::Windows::Forms::CheckBox());
			   this->spaceSceneRadioButton = (gcnew System::Windows::Forms::RadioButton());
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->specularStrengthTrackBar))->BeginInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rTrackBar))->BeginInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->gTrackBar))->BeginInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->bTrackBar))->BeginInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->frquencyTrackBar))->BeginInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->amplitudeTrackBar))->BeginInit();
			   this->SuspendLayout();
			   // 
			   // moveLightRadioButton
			   // 
			   this->moveLightRadioButton->AutoSize = true;
			   this->moveLightRadioButton->Checked = true;
			   this->moveLightRadioButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold,
				   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			   this->moveLightRadioButton->Location = System::Drawing::Point(12, 9);
			   this->moveLightRadioButton->Name = L"moveLightRadioButton";
			   this->moveLightRadioButton->Size = System::Drawing::Size(103, 20);
			   this->moveLightRadioButton->TabIndex = 0;
			   this->moveLightRadioButton->TabStop = true;
			   this->moveLightRadioButton->Text = L"Move Light";
			   this->moveLightRadioButton->UseVisualStyleBackColor = true;
			   this->moveLightRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::moveLightRadioButton_CheckedChanged);
			   // 
			   // resetLightPositionButton
			   // 
			   this->resetLightPositionButton->Location = System::Drawing::Point(16, 36);
			   this->resetLightPositionButton->Name = L"resetLightPositionButton";
			   this->resetLightPositionButton->Size = System::Drawing::Size(168, 31);
			   this->resetLightPositionButton->TabIndex = 1;
			   this->resetLightPositionButton->Text = L"Reset Light Position";
			   this->resetLightPositionButton->UseVisualStyleBackColor = true;
			   this->resetLightPositionButton->Click += gcnew System::EventHandler(this, &ToolWindow::resetLightPositionButton_Click);
			   // 
			   // specularLabel
			   // 
			   this->specularLabel->AutoSize = true;
			   this->specularLabel->Location = System::Drawing::Point(15, 81);
			   this->specularLabel->Name = L"specularLabel";
			   this->specularLabel->Size = System::Drawing::Size(113, 80);
			   this->specularLabel->TabIndex = 2;
			   this->specularLabel->Text = L"Specular Strength\r\n\r\n\r\n\r\nSpecular Color";
			   // 
			   // specularStrengthTrackBar
			   // 
			   this->specularStrengthTrackBar->Location = System::Drawing::Point(134, 73);
			   this->specularStrengthTrackBar->Maximum = 128;
			   this->specularStrengthTrackBar->Minimum = 1;
			   this->specularStrengthTrackBar->Name = L"specularStrengthTrackBar";
			   this->specularStrengthTrackBar->Size = System::Drawing::Size(336, 56);
			   this->specularStrengthTrackBar->TabIndex = 3;
			   this->specularStrengthTrackBar->Value = 4;
			   this->specularStrengthTrackBar->Scroll += gcnew System::EventHandler(this, &ToolWindow::specularStrengthTrackBar_Scroll);
			   // 
			   // specularStrengthValueLabel
			   // 
			   this->specularStrengthValueLabel->AutoSize = true;
			   this->specularStrengthValueLabel->Location = System::Drawing::Point(476, 81);
			   this->specularStrengthValueLabel->Name = L"specularStrengthValueLabel";
			   this->specularStrengthValueLabel->Size = System::Drawing::Size(28, 16);
			   this->specularStrengthValueLabel->TabIndex = 4;
			   this->specularStrengthValueLabel->Text = L"000";
			   // 
			   // rTrackBar
			   // 
			   this->rTrackBar->Location = System::Drawing::Point(134, 135);
			   this->rTrackBar->Maximum = 300;
			   this->rTrackBar->Name = L"rTrackBar";
			   this->rTrackBar->Size = System::Drawing::Size(336, 56);
			   this->rTrackBar->TabIndex = 6;
			   this->rTrackBar->Value = 100;
			   this->rTrackBar->Scroll += gcnew System::EventHandler(this, &ToolWindow::rTrackBar_Scroll);
			   // 
			   // rValueLabel
			   // 
			   this->rValueLabel->AutoSize = true;
			   this->rValueLabel->Location = System::Drawing::Point(476, 145);
			   this->rValueLabel->Name = L"rValueLabel";
			   this->rValueLabel->Size = System::Drawing::Size(31, 16);
			   this->rValueLabel->TabIndex = 7;
			   this->rValueLabel->Text = L"0.00";
			   // 
			   // gTrackBar
			   // 
			   this->gTrackBar->Location = System::Drawing::Point(134, 197);
			   this->gTrackBar->Maximum = 300;
			   this->gTrackBar->Name = L"gTrackBar";
			   this->gTrackBar->Size = System::Drawing::Size(336, 56);
			   this->gTrackBar->TabIndex = 9;
			   this->gTrackBar->Value = 100;
			   this->gTrackBar->Scroll += gcnew System::EventHandler(this, &ToolWindow::gTrackBar_Scroll);
			   // 
			   // gValueLabel
			   // 
			   this->gValueLabel->AutoSize = true;
			   this->gValueLabel->Location = System::Drawing::Point(476, 207);
			   this->gValueLabel->Name = L"gValueLabel";
			   this->gValueLabel->Size = System::Drawing::Size(31, 16);
			   this->gValueLabel->TabIndex = 10;
			   this->gValueLabel->Text = L"0.00";
			   // 
			   // bTrackBar
			   // 
			   this->bTrackBar->Location = System::Drawing::Point(134, 259);
			   this->bTrackBar->Maximum = 300;
			   this->bTrackBar->Name = L"bTrackBar";
			   this->bTrackBar->Size = System::Drawing::Size(336, 56);
			   this->bTrackBar->TabIndex = 12;
			   this->bTrackBar->Value = 100;
			   this->bTrackBar->Scroll += gcnew System::EventHandler(this, &ToolWindow::bTrackBar_Scroll);
			   // 
			   // bValueLabel
			   // 
			   this->bValueLabel->AutoSize = true;
			   this->bValueLabel->Location = System::Drawing::Point(476, 269);
			   this->bValueLabel->Name = L"bValueLabel";
			   this->bValueLabel->Size = System::Drawing::Size(31, 16);
			   this->bValueLabel->TabIndex = 13;
			   this->bValueLabel->Text = L"0.00";
			   // 
			   // transformRadioButton
			   // 
			   this->transformRadioButton->AutoSize = true;
			   this->transformRadioButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold,
				   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			   this->transformRadioButton->Location = System::Drawing::Point(12, 321);
			   this->transformRadioButton->Name = L"transformRadioButton";
			   this->transformRadioButton->Size = System::Drawing::Size(98, 20);
			   this->transformRadioButton->TabIndex = 14;
			   this->transformRadioButton->TabStop = true;
			   this->transformRadioButton->Text = L"Transform";
			   this->transformRadioButton->UseVisualStyleBackColor = true;
			   this->transformRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::transformRadioButton_CheckedChanged);
			   // 
			   // resetTransformButton
			   // 
			   this->resetTransformButton->Location = System::Drawing::Point(16, 347);
			   this->resetTransformButton->Name = L"resetTransformButton";
			   this->resetTransformButton->Size = System::Drawing::Size(168, 31);
			   this->resetTransformButton->TabIndex = 15;
			   this->resetTransformButton->Text = L"Reset Transform";
			   this->resetTransformButton->UseVisualStyleBackColor = true;
			   // 
			   // waterSceneRadioButton
			   // 
			   this->waterSceneRadioButton->AutoSize = true;
			   this->waterSceneRadioButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold,
				   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			   this->waterSceneRadioButton->Location = System::Drawing::Point(12, 424);
			   this->waterSceneRadioButton->Name = L"waterSceneRadioButton";
			   this->waterSceneRadioButton->Size = System::Drawing::Size(117, 20);
			   this->waterSceneRadioButton->TabIndex = 16;
			   this->waterSceneRadioButton->TabStop = true;
			   this->waterSceneRadioButton->Text = L"Water Scene";
			   this->waterSceneRadioButton->UseVisualStyleBackColor = true;
			   this->waterSceneRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::waterSceneRadioButton_CheckedChanged);
			   // 
			   // rLabel
			   // 
			   this->rLabel->AutoSize = true;
			   this->rLabel->Location = System::Drawing::Point(120, 145);
			   this->rLabel->Name = L"rLabel";
			   this->rLabel->Size = System::Drawing::Size(17, 16);
			   this->rLabel->TabIndex = 17;
			   this->rLabel->Text = L"R";
			   // 
			   // gLabel
			   // 
			   this->gLabel->AutoSize = true;
			   this->gLabel->Location = System::Drawing::Point(120, 207);
			   this->gLabel->Name = L"gLabel";
			   this->gLabel->Size = System::Drawing::Size(17, 16);
			   this->gLabel->TabIndex = 18;
			   this->gLabel->Text = L"G";
			   // 
			   // bLabel
			   // 
			   this->bLabel->AutoSize = true;
			   this->bLabel->Location = System::Drawing::Point(120, 269);
			   this->bLabel->Name = L"bLabel";
			   this->bLabel->Size = System::Drawing::Size(16, 16);
			   this->bLabel->TabIndex = 19;
			   this->bLabel->Text = L"B";
			   // 
			   // translateCheckBox
			   // 
			   this->translateCheckBox->AutoSize = true;
			   this->translateCheckBox->Location = System::Drawing::Point(18, 385);
			   this->translateCheckBox->Name = L"translateCheckBox";
			   this->translateCheckBox->Size = System::Drawing::Size(86, 20);
			   this->translateCheckBox->TabIndex = 20;
			   this->translateCheckBox->Text = L"Translate";
			   this->translateCheckBox->UseVisualStyleBackColor = true;
			   // 
			   // rotateCheckBox
			   // 
			   this->rotateCheckBox->AutoSize = true;
			   this->rotateCheckBox->Location = System::Drawing::Point(188, 385);
			   this->rotateCheckBox->Name = L"rotateCheckBox";
			   this->rotateCheckBox->Size = System::Drawing::Size(69, 20);
			   this->rotateCheckBox->TabIndex = 21;
			   this->rotateCheckBox->Text = L"Rotate";
			   this->rotateCheckBox->UseVisualStyleBackColor = true;
			   // 
			   // scaleCheckBox
			   // 
			   this->scaleCheckBox->AutoSize = true;
			   this->scaleCheckBox->Location = System::Drawing::Point(353, 385);
			   this->scaleCheckBox->Name = L"scaleCheckBox";
			   this->scaleCheckBox->Size = System::Drawing::Size(64, 20);
			   this->scaleCheckBox->TabIndex = 22;
			   this->scaleCheckBox->Text = L"Scale";
			   this->scaleCheckBox->UseVisualStyleBackColor = true;
			   // 
			   // frequencyLabel
			   // 
			   this->frequencyLabel->AutoSize = true;
			   this->frequencyLabel->Location = System::Drawing::Point(12, 451);
			   this->frequencyLabel->Name = L"frequencyLabel";
			   this->frequencyLabel->Size = System::Drawing::Size(71, 16);
			   this->frequencyLabel->TabIndex = 23;
			   this->frequencyLabel->Text = L"Frequency";
			   // 
			   // frquencyTrackBar
			   // 
			   this->frquencyTrackBar->Location = System::Drawing::Point(12, 475);
			   this->frquencyTrackBar->Name = L"frquencyTrackBar";
			   this->frquencyTrackBar->Size = System::Drawing::Size(458, 56);
			   this->frquencyTrackBar->TabIndex = 24;
			   // 
			   // frequencyValueLabel
			   // 
			   this->frequencyValueLabel->AutoSize = true;
			   this->frequencyValueLabel->Location = System::Drawing::Point(476, 486);
			   this->frequencyValueLabel->Name = L"frequencyValueLabel";
			   this->frequencyValueLabel->Size = System::Drawing::Size(31, 16);
			   this->frequencyValueLabel->TabIndex = 25;
			   this->frequencyValueLabel->Text = L"0.00";
			   // 
			   // amplitudeValueLabel
			   // 
			   this->amplitudeValueLabel->AutoSize = true;
			   this->amplitudeValueLabel->Location = System::Drawing::Point(476, 548);
			   this->amplitudeValueLabel->Name = L"amplitudeValueLabel";
			   this->amplitudeValueLabel->Size = System::Drawing::Size(31, 16);
			   this->amplitudeValueLabel->TabIndex = 28;
			   this->amplitudeValueLabel->Text = L"0.00";
			   // 
			   // amplitudeTrackBar
			   // 
			   this->amplitudeTrackBar->Location = System::Drawing::Point(12, 537);
			   this->amplitudeTrackBar->Name = L"amplitudeTrackBar";
			   this->amplitudeTrackBar->Size = System::Drawing::Size(458, 56);
			   this->amplitudeTrackBar->TabIndex = 27;
			   // 
			   // amplitudeLabel
			   // 
			   this->amplitudeLabel->AutoSize = true;
			   this->amplitudeLabel->Location = System::Drawing::Point(12, 513);
			   this->amplitudeLabel->Name = L"amplitudeLabel";
			   this->amplitudeLabel->Size = System::Drawing::Size(67, 16);
			   this->amplitudeLabel->TabIndex = 26;
			   this->amplitudeLabel->Text = L"Amplitude";
			   // 
			   // tintBlueCheckBox
			   // 
			   this->tintBlueCheckBox->AutoSize = true;
			   this->tintBlueCheckBox->Location = System::Drawing::Point(182, 573);
			   this->tintBlueCheckBox->Name = L"tintBlueCheckBox";
			   this->tintBlueCheckBox->Size = System::Drawing::Size(81, 20);
			   this->tintBlueCheckBox->TabIndex = 30;
			   this->tintBlueCheckBox->Text = L"Tint Blue";
			   this->tintBlueCheckBox->UseVisualStyleBackColor = true;
			   // 
			   // wireframeRenderCheckBox
			   // 
			   this->wireframeRenderCheckBox->AutoSize = true;
			   this->wireframeRenderCheckBox->Location = System::Drawing::Point(12, 573);
			   this->wireframeRenderCheckBox->Name = L"wireframeRenderCheckBox";
			   this->wireframeRenderCheckBox->Size = System::Drawing::Size(139, 20);
			   this->wireframeRenderCheckBox->TabIndex = 29;
			   this->wireframeRenderCheckBox->Text = L"Wireframe Render";
			   this->wireframeRenderCheckBox->UseVisualStyleBackColor = true;
			   // 
			   // spaceSceneRadioButton
			   // 
			   this->spaceSceneRadioButton->AutoSize = true;
			   this->spaceSceneRadioButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold,
				   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			   this->spaceSceneRadioButton->Location = System::Drawing::Point(11, 610);
			   this->spaceSceneRadioButton->Name = L"spaceSceneRadioButton";
			   this->spaceSceneRadioButton->Size = System::Drawing::Size(121, 20);
			   this->spaceSceneRadioButton->TabIndex = 31;
			   this->spaceSceneRadioButton->TabStop = true;
			   this->spaceSceneRadioButton->Text = L"Space Scene";
			   this->spaceSceneRadioButton->UseVisualStyleBackColor = true;
			   this->spaceSceneRadioButton->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::spaceSceneRadioButton_CheckedChanged);
			   // 
			   // ToolWindow
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(520, 644);
			   this->Controls->Add(this->spaceSceneRadioButton);
			   this->Controls->Add(this->tintBlueCheckBox);
			   this->Controls->Add(this->wireframeRenderCheckBox);
			   this->Controls->Add(this->amplitudeValueLabel);
			   this->Controls->Add(this->amplitudeTrackBar);
			   this->Controls->Add(this->amplitudeLabel);
			   this->Controls->Add(this->frequencyValueLabel);
			   this->Controls->Add(this->frquencyTrackBar);
			   this->Controls->Add(this->frequencyLabel);
			   this->Controls->Add(this->scaleCheckBox);
			   this->Controls->Add(this->rotateCheckBox);
			   this->Controls->Add(this->translateCheckBox);
			   this->Controls->Add(this->bLabel);
			   this->Controls->Add(this->gLabel);
			   this->Controls->Add(this->rLabel);
			   this->Controls->Add(this->waterSceneRadioButton);
			   this->Controls->Add(this->resetTransformButton);
			   this->Controls->Add(this->moveLightRadioButton);
			   this->Controls->Add(this->transformRadioButton);
			   this->Controls->Add(this->resetLightPositionButton);
			   this->Controls->Add(this->bValueLabel);
			   this->Controls->Add(this->specularLabel);
			   this->Controls->Add(this->bTrackBar);
			   this->Controls->Add(this->specularStrengthTrackBar);
			   this->Controls->Add(this->specularStrengthValueLabel);
			   this->Controls->Add(this->gValueLabel);
			   this->Controls->Add(this->rTrackBar);
			   this->Controls->Add(this->gTrackBar);
			   this->Controls->Add(this->rValueLabel);
			   this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			   this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			   this->Name = L"ToolWindow";
			   this->Text = L"Final Project";
			   this->TopMost = true;
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->specularStrengthTrackBar))->EndInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rTrackBar))->EndInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->gTrackBar))->EndInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->bTrackBar))->EndInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->frquencyTrackBar))->EndInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->amplitudeTrackBar))->EndInit();
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion

#pragma region Game Mode Select
	private: System::Void moveLightRadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
	{
		GameController::GetInstance().gameMode = GameMode::MoveLight;
	}

	private: System::Void transformRadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
	{
		GameController::GetInstance().gameMode = GameMode::Transform;
	}

	private: System::Void waterSceneRadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
	{
		GameController::GetInstance().gameMode = GameMode::WaterScene;
	}

	private: System::Void spaceSceneRadioButton_CheckedChanged(System::Object^ sender, System::EventArgs^ e) 
	{
		GameController::GetInstance().gameMode = GameMode::SpaceScene;
	}
#pragma endregion

#pragma region Move Light
	private: System::Void resetLightPositionButton_Click(System::Object^ sender, System::EventArgs^ e)
	{
		GameController::GetInstance().ResetLightPosition();
	}

	private: System::Void UpdateSpecularValues()
	{
		GameController::GetInstance().specularStrength = specularStrengthTrackBar->Value;
		specularStrengthValueLabel->Text = specularStrengthTrackBar->Value.ToString();

		float r = 3.f * rTrackBar->Value / rTrackBar->Maximum;
		float g = 3.f * gTrackBar->Value / gTrackBar->Maximum;
		float b = 3.f * bTrackBar->Value / bTrackBar->Maximum;
		GameController::GetInstance().specularColor = { r, g, b };
		rValueLabel->Text = r.ToString("F2");
		gValueLabel->Text = g.ToString("F2");
		bValueLabel->Text = b.ToString("F2");
	}

	private: System::Void specularStrengthTrackBar_Scroll(System::Object^ sender, System::EventArgs^ e)
	{
		UpdateSpecularValues();
	}

	private: System::Void rTrackBar_Scroll(System::Object^ sender, System::EventArgs^ e)
	{
		UpdateSpecularValues();
	}

	private: System::Void gTrackBar_Scroll(System::Object^ sender, System::EventArgs^ e)
	{
		UpdateSpecularValues();
	}

	private: System::Void bTrackBar_Scroll(System::Object^ sender, System::EventArgs^ e)
	{
		UpdateSpecularValues();
	}
#pragma endregion
	};
}