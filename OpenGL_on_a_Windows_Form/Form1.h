#pragma once

//#include "OpenGL.h"
#include "VisualizationPanel.h"
#include "RawDataVisualization.h"
#include "DetailVisualization.h"
#include "HistogramVisualization.h"
namespace OpenGL_on_a_Windows_Form 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace OpenGLForm;
	public ref class Form1 : public System::Windows::Forms::Form 
	{
	public:
		Form1(void)
		{
			InitializeComponent();

			ReadCSV read_csv;
			//char file_name[200] = "file_list_MRT_rain.txt";
			//char file_name[200] = "file_list_MRT.txt";
			//read_csv.read_file_list(file_name);;
			//read_csv.read_holiday_event("holiday_event.txt");
			read_csv.read_file_all();
			read_csv.read_MRT_position("MRT_position.txt");

			Preprocessing_Data preprocessing_data;

			preprocessing_data.Initial_selection_flag(this->residential_in->Checked,
													  this->residential_out->Checked,
													  this->work_school_in->Checked,
													  this->work_school_out->Checked,
													  this->tourism_in->Checked,
													  this->tourism_out->Checked);

			preprocessing_data.MRT_position = read_csv.MRT_position;
			preprocessing_data.month_vec = read_csv.month_vec;
			preprocessing_data.start_on_2D(read_csv.hour_amount,read_csv.day_amount);
			//preprocessing_data.start3(read_csv.day_amount,read_csv.hour_amount,trackBar1->Value);


			histogram = gcnew HistogramVisualization(this,this->panel3,panel3->Width,panel3->Height,read_csv,preprocessing_data);
			rawData = gcnew RawDataVisualization(this,this->panel1,panel1->Width,panel1->Height,read_csv,preprocessing_data);
			//detail = gcnew DetailVisualization(this,this->panel2,panel2->Width,panel2->Height,read_csv,preprocessing_data);

		}
	protected:

		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::ComponentModel::IContainer^  components;
	protected: 
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Button^  start;


	private: System::Windows::Forms::Button^  button2;

	private:

		RawDataVisualization ^ rawData;
		DetailVisualization ^ detail;
		HistogramVisualization ^ histogram;

	private: System::Windows::Forms::TrackBar^  trackBar1;
	
	private: System::Boolean Move_3_21_flag,Down_3_21_flag;
	private: System::Boolean Move_12_horiz_flag,Down_12_horiz_flag;
	private: System::Boolean Move_12_vert_flag,Down_12_vert_flag;
	private: System::Int32 last_x,last_y;

	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Boolean waiting_flag;
	private: System::Windows::Forms::CheckBox^  residential_in;
	private: System::Windows::Forms::CheckBox^  residential_out;

	private: System::Windows::Forms::Label^  feature_selection_label;
	private: System::Windows::Forms::CheckBox^  work_school_in;
	private: System::Windows::Forms::CheckBox^  work_school_out;
	private: System::Windows::Forms::Label^  cluster_label;
	private: System::Boolean start_flag;

	private: System::Windows::Forms::Button^  load_csv;
	private: System::Windows::Forms::Label^  file_directory;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::Windows::Forms::CheckBox^  tourism_in;
	private: System::Windows::Forms::CheckBox^  tourism_out;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::ComboBox^  view_select_left;
	private: System::Windows::Forms::Label^  view_left_label;
	private: System::Windows::Forms::Label^  view_right_label;
	private: System::Windows::Forms::ComboBox^  view_select_right;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  RightViewClear;



	public:  System::Windows::Forms::OpenFileDialog ofdOpen;
	//private: System::String^ file_string;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->start = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->feature_selection_label = (gcnew System::Windows::Forms::Label());
			this->residential_in = (gcnew System::Windows::Forms::CheckBox());
			this->residential_out = (gcnew System::Windows::Forms::CheckBox());
			this->work_school_in = (gcnew System::Windows::Forms::CheckBox());
			this->work_school_out = (gcnew System::Windows::Forms::CheckBox());
			this->tourism_in = (gcnew System::Windows::Forms::CheckBox());
			this->tourism_out = (gcnew System::Windows::Forms::CheckBox());
			this->cluster_label = (gcnew System::Windows::Forms::Label());
			this->load_csv = (gcnew System::Windows::Forms::Button());
			this->file_directory = (gcnew System::Windows::Forms::Label());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->view_select_left = (gcnew System::Windows::Forms::ComboBox());
			this->view_left_label = (gcnew System::Windows::Forms::Label());
			this->view_right_label = (gcnew System::Windows::Forms::Label());
			this->view_select_right = (gcnew System::Windows::Forms::ComboBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->RightViewClear = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(718, 36);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(780, 1006);
			this->panel1->TabIndex = 0;
			this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::panel1_Paint);
			this->panel1->MouseEnter += gcnew System::EventHandler(this, &Form1::panel1_MouseEnter);
			this->panel1->MouseHover += gcnew System::EventHandler(this, &Form1::panel1_MouseHover);
			this->panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::panel1_MouseMove);
			// 
			// panel2
			// 
			this->panel2->Location = System::Drawing::Point(1595, 253);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(309, 149);
			this->panel2->TabIndex = 1;
			this->panel2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::panel2_Paint);
			this->panel2->MouseEnter += gcnew System::EventHandler(this, &Form1::panel2_MouseEnter);
			this->panel2->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::panel2_MouseMove);
			// 
			// panel3
			// 
			this->panel3->Location = System::Drawing::Point(12, 36);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(700, 1006);
			this->panel3->TabIndex = 2;
			this->panel3->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::panel3_Paint);
			this->panel3->MouseEnter += gcnew System::EventHandler(this, &Form1::panel3_MouseEnter);
			this->panel3->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::panel3_MouseMove);
			// 
			// start
			// 
			this->start->Location = System::Drawing::Point(1763, 38);
			this->start->Name = L"start";
			this->start->Size = System::Drawing::Size(134, 67);
			this->start->TabIndex = 4;
			this->start->Text = L"Start";
			this->start->UseVisualStyleBackColor = true;
			this->start->Click += gcnew System::EventHandler(this, &Form1::start_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(1504, 38);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(71, 38);
			this->button2->TabIndex = 5;
			this->button2->Text = L"Station Clear";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// trackBar1
			// 
			this->trackBar1->LargeChange = 1;
			this->trackBar1->Location = System::Drawing::Point(1586, 202);
			this->trackBar1->Maximum = 50;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(196, 45);
			this->trackBar1->TabIndex = 6;
			this->trackBar1->Value = 5;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &Form1::trackBar1_Scroll);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(1777, 202);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(24, 22);
			this->textBox1->TabIndex = 8;
			this->textBox1->Text = L"5";
			// 
			// feature_selection_label
			// 
			this->feature_selection_label->AutoSize = true;
			this->feature_selection_label->Location = System::Drawing::Point(1593, 14);
			this->feature_selection_label->Name = L"feature_selection_label";
			this->feature_selection_label->Size = System::Drawing::Size(84, 12);
			this->feature_selection_label->TabIndex = 11;
			this->feature_selection_label->Text = L"Feature Selection";
			// 
			// residential_in
			// 
			this->residential_in->AutoSize = true;
			this->residential_in->Checked = true;
			this->residential_in->CheckState = System::Windows::Forms::CheckState::Checked;
			this->residential_in->Location = System::Drawing::Point(1595, 38);
			this->residential_in->Name = L"residential_in";
			this->residential_in->Size = System::Drawing::Size(96, 16);
			this->residential_in->TabIndex = 9;
			this->residential_in->Text = L"Redisential (In)";
			this->residential_in->UseVisualStyleBackColor = true;
			this->residential_in->CheckedChanged += gcnew System::EventHandler(this, &Form1::residential_in_CheckedChanged);
			// 
			// residential_out
			// 
			this->residential_out->AutoSize = true;
			this->residential_out->Checked = true;
			this->residential_out->CheckState = System::Windows::Forms::CheckState::Checked;
			this->residential_out->Location = System::Drawing::Point(1595, 60);
			this->residential_out->Name = L"residential_out";
			this->residential_out->Size = System::Drawing::Size(103, 16);
			this->residential_out->TabIndex = 10;
			this->residential_out->Text = L"Redisential (Out)";
			this->residential_out->UseVisualStyleBackColor = true;
			this->residential_out->CheckedChanged += gcnew System::EventHandler(this, &Form1::residential_out_CheckedChanged);
			// 
			// work_school_in
			// 
			this->work_school_in->AutoSize = true;
			this->work_school_in->Checked = true;
			this->work_school_in->CheckState = System::Windows::Forms::CheckState::Checked;
			this->work_school_in->Location = System::Drawing::Point(1595, 82);
			this->work_school_in->Name = L"work_school_in";
			this->work_school_in->Size = System::Drawing::Size(107, 16);
			this->work_school_in->TabIndex = 12;
			this->work_school_in->Text = L"Work/School (In)";
			this->work_school_in->UseVisualStyleBackColor = true;
			this->work_school_in->CheckedChanged += gcnew System::EventHandler(this, &Form1::work_school_in_CheckedChanged);
			// 
			// work_school_out
			// 
			this->work_school_out->AutoSize = true;
			this->work_school_out->Checked = true;
			this->work_school_out->CheckState = System::Windows::Forms::CheckState::Checked;
			this->work_school_out->Location = System::Drawing::Point(1595, 104);
			this->work_school_out->Name = L"work_school_out";
			this->work_school_out->Size = System::Drawing::Size(114, 16);
			this->work_school_out->TabIndex = 13;
			this->work_school_out->Text = L"Work/School (Out)";
			this->work_school_out->UseVisualStyleBackColor = true;
			this->work_school_out->CheckedChanged += gcnew System::EventHandler(this, &Form1::work_school_out_CheckedChanged);
			// 
			// tourism_in
			// 
			this->tourism_in->AutoSize = true;
			this->tourism_in->Checked = true;
			this->tourism_in->CheckState = System::Windows::Forms::CheckState::Checked;
			this->tourism_in->Location = System::Drawing::Point(1595, 126);
			this->tourism_in->Name = L"tourism_in";
			this->tourism_in->Size = System::Drawing::Size(84, 16);
			this->tourism_in->TabIndex = 18;
			this->tourism_in->Text = L"Tourism (In)";
			this->tourism_in->UseVisualStyleBackColor = true;
			this->tourism_in->CheckedChanged += gcnew System::EventHandler(this, &Form1::tourism_in_CheckedChanged);
			// 
			// tourism_out
			// 
			this->tourism_out->AutoSize = true;
			this->tourism_out->Checked = true;
			this->tourism_out->CheckState = System::Windows::Forms::CheckState::Checked;
			this->tourism_out->Location = System::Drawing::Point(1595, 148);
			this->tourism_out->Name = L"tourism_out";
			this->tourism_out->Size = System::Drawing::Size(91, 16);
			this->tourism_out->TabIndex = 19;
			this->tourism_out->Text = L"Tourism (Out)";
			this->tourism_out->UseVisualStyleBackColor = true;
			this->tourism_out->CheckedChanged += gcnew System::EventHandler(this, &Form1::tourism_out_CheckedChanged);
			// 
			// cluster_label
			// 
			this->cluster_label->AutoSize = true;
			this->cluster_label->Location = System::Drawing::Point(1593, 177);
			this->cluster_label->Name = L"cluster_label";
			this->cluster_label->Size = System::Drawing::Size(129, 12);
			this->cluster_label->TabIndex = 14;
			this->cluster_label->Text = L"Cluster Number (k-means)";
			// 
			// load_csv
			// 
			this->load_csv->Location = System::Drawing::Point(1780, 130);
			this->load_csv->Name = L"load_csv";
			this->load_csv->Size = System::Drawing::Size(102, 32);
			this->load_csv->TabIndex = 16;
			this->load_csv->Text = L"Load csv File";
			this->load_csv->UseVisualStyleBackColor = true;
			this->load_csv->Click += gcnew System::EventHandler(this, &Form1::load_csv_Click);
			// 
			// file_directory
			// 
			this->file_directory->AutoSize = true;
			this->file_directory->Location = System::Drawing::Point(1669, 600);
			this->file_directory->Name = L"file_directory";
			this->file_directory->Size = System::Drawing::Size(8, 12);
			this->file_directory->TabIndex = 17;
			this->file_directory->Text = L" ";
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerReportsProgress = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker1_DoWork);
			this->backgroundWorker1->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &Form1::backgroundWorker1_RunWorkerCompleted);
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"All", L"Weekday", L"Weekend", L"Holiday"});
			this->comboBox1->Location = System::Drawing::Point(591, 10);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 20);
			this->comboBox1->TabIndex = 20;
			this->comboBox1->Text = L"All";
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// view_select_left
			// 
			this->view_select_left->Cursor = System::Windows::Forms::Cursors::Default;
			this->view_select_left->FormattingEnabled = true;
			this->view_select_left->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"Time"});
			this->view_select_left->Location = System::Drawing::Point(240, 10);
			this->view_select_left->Name = L"view_select_left";
			this->view_select_left->Size = System::Drawing::Size(121, 20);
			this->view_select_left->TabIndex = 21;
			this->view_select_left->Text = L"Time";
			this->view_select_left->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::view_select_left_SelectedIndexChanged);
			// 
			// view_left_label
			// 
			this->view_left_label->AutoSize = true;
			this->view_left_label->Font = (gcnew System::Drawing::Font(L"新細明體", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->view_left_label->Location = System::Drawing::Point(200, 11);
			this->view_left_label->Name = L"view_left_label";
			this->view_left_label->Size = System::Drawing::Size(34, 15);
			this->view_left_label->TabIndex = 22;
			this->view_left_label->Text = L"view";
			// 
			// view_right_label
			// 
			this->view_right_label->AutoSize = true;
			this->view_right_label->Font = (gcnew System::Drawing::Font(L"新細明體", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->view_right_label->Location = System::Drawing::Point(1016, 11);
			this->view_right_label->Name = L"view_right_label";
			this->view_right_label->Size = System::Drawing::Size(34, 15);
			this->view_right_label->TabIndex = 23;
			this->view_right_label->Text = L"view";
			// 
			// view_select_right
			// 
			this->view_select_right->FormattingEnabled = true;
			this->view_select_right->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Time", L"Station", L"LineChart"});
			this->view_select_right->Location = System::Drawing::Point(1056, 10);
			this->view_select_right->Name = L"view_select_right";
			this->view_select_right->Size = System::Drawing::Size(121, 20);
			this->view_select_right->TabIndex = 24;
			this->view_select_right->Text = L"Station";
			this->view_select_right->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::view_select_right_SelectedIndexChanged);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(1504, 408);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(400, 634);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Click += gcnew System::EventHandler(this, &Form1::pictureBox1_Click);
			// 
			// RightViewClear
			// 
			this->RightViewClear->Location = System::Drawing::Point(1304, 8);
			this->RightViewClear->Name = L"RightViewClear";
			this->RightViewClear->Size = System::Drawing::Size(75, 23);
			this->RightViewClear->TabIndex = 25;
			this->RightViewClear->Text = L"View Clear";
			this->RightViewClear->UseVisualStyleBackColor = true;
			this->RightViewClear->Click += gcnew System::EventHandler(this, &Form1::RightViewClear_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1916, 1054);
			this->Controls->Add(this->RightViewClear);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->view_select_right);
			this->Controls->Add(this->view_right_label);
			this->Controls->Add(this->view_left_label);
			this->Controls->Add(this->view_select_left);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->tourism_out);
			this->Controls->Add(this->tourism_in);
			this->Controls->Add(this->file_directory);
			this->Controls->Add(this->load_csv);
			this->Controls->Add(this->cluster_label);
			this->Controls->Add(this->work_school_out);
			this->Controls->Add(this->work_school_in);
			this->Controls->Add(this->feature_selection_label);
			this->Controls->Add(this->residential_out);
			this->Controls->Add(this->residential_in);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->start);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->Name = L"Form1";
			this->Text = L"OpenGL on a Windows Form using Managed C++";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseDown);
			this->MouseEnter += gcnew System::EventHandler(this, &Form1::Form1_MouseEnter);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseMove_1);
			this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseUp);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
		void MarshalString ( System::String ^ s, string& os ) {
		   using namespace Runtime::InteropServices;
		   const char* chars = 
			  (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		   os = chars;
		   Marshal::FreeHGlobal(IntPtr((void*)chars));
		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(rawData)
					 rawData->Render(panel1->Width,panel1->Height);
				 if(detail)
					 detail->Render(panel2->Width,panel2->Height);
				 if(histogram)
					 histogram->Render(panel3->Width,panel3->Height);

				 if(Move_3_21_flag==false && Move_12_horiz_flag==false && Move_12_vert_flag==false) 
					 this->Cursor = System::Windows::Forms::Cursors::Default;

			 }

	private: System::Void start_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 backgroundWorker1->RunWorkerAsync();

				 histogram->clear();
				 //rawData->clear();
				 histogram_index.clear();
				 detail->clear();

				 //read_csv.test_image();///////////////////////////////////

				preprocessing_data.Initial_selection_flag(this->residential_in->Checked,
														  this->residential_out->Checked,
														  this->work_school_in->Checked,
														  this->work_school_out->Checked,
														  this->tourism_in->Checked,
														  this->tourism_out->Checked);
				 
				//preprocessing_data.select_station.clear();

				delete this->pictureBox1->Image;
				if( remove( "MRT_Map2.jpg" ) != 0 )
					System::Windows::Forms::MessageBox::Show("Error deleting file");
					//perror( "Error deleting file" );
				//else
				//	System::Windows::Forms::MessageBox::Show("File successfully deleted");

				preprocessing_data.circle_MRT_station();

				this->pictureBox1->Image = Image::FromFile("MRT_map2.jpg");
				

				preprocessing_data.start3(read_csv.day_amount,read_csv.hour_amount,trackBar1->Value);

				preprocessing_data.start_flag = true;

				view_select_right->SelectedIndex = 2;
				preprocessing_data.view_select_right_index = 2;
			 }

    private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 rawData->clear();
				 rawData->clear_view();

				 view_select_right->SelectedIndex = 1;
				 preprocessing_data.view_select_right_index = 1;
			 }

	private: System::Void RightViewClear_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 rawData->clear_view();
			 }

	private: System::Void Detail_Clear_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 detail->clear();
			 }

	private: System::Void load_csv_Click(System::Object^  sender, System::EventArgs^  e) {
				 System::Windows::Forms::OpenFileDialog^ ofdOpen=gcnew System::Windows::Forms::OpenFileDialog();


				 if ((ofdOpen->InitialDirectory == NULL.ToString())) 
				 {
					ofdOpen->InitialDirectory = "E:Github/csv_data/"; // 語音檔預設開啟目錄
				 }

				 ofdOpen->Filter = 
						//"Audio files (*.wav; *.mpa; *.mp2; *.mp3; *.au; *.aif; *.aiff; *.snd; *.wma)|*.wav; *.mpa; *.mp2; *.mp3; *.au; *.aif; *.aiff; *.snd; *.wma|" +
						//"Video Files (*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v; *.wmv)|*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v; *.wmv|" +
						//"MIDI Files (*.mid, *.midi, *.rmKKTimeri)|*.mid; *.midi; *.rmi|" +
						//"Image Files (*.jpg, *.bmp, *.gif, *.tga)|*.jpg; *.bmp; *.gif; *.tga|" +
						"CSV Files (*.csv)|*.csv|" +
						"All Files (*.*)|*.*";
						ofdOpen->Title = "Open CSV file";
						ofdOpen->Multiselect = true; // 允許選取多檔案

				 if (ofdOpen->ShowDialog(this) == System::Windows::Forms::DialogResult::Cancel) 
				 {
				 		// 使用者沒有選檔案
						return;
				 }
		 
				 System::String^ Filename = ofdOpen->FileName; // 選擇的完整路徑

				 file_directory->Text = "" + Filename;
				 //System::Windows::Forms::MessageBox::Show(Filename);

				 read_csv.clear();
				 //System::String to string, string to char*
				 string file;
				 MarshalString(Filename, file);
				 char file_char[200];
				 strcpy(file_char,file.c_str());
				 read_csv.read_raw_data(file_char);

				 //clear the mat & reset the flag & start
				 detail->clear();
				 rawData->clear();
				 histogram->clear();
				 histogram_position_table.clear();
				 //preprocessing_data.Initial_selection_flag(this->Gravity_Norm->Checked,this->Linear_Acceleration_Norm->Checked,
				 //											this->Gyroscope_Norm->Checked,this->First_Order_of_Distance->Checked);
				 //Gravity_Norm->Checked = this->Gravity_Norm->Checked;
				 //Linear_Acceleration_Norm->Checked = this->Linear_Acceleration_Norm->Checked;
				 //Gyroscope_Norm->Checked = this->Gyroscope_Norm->Checked;
				 //First_Order_of_Distance->Checked = this->First_Order_of_Distance->Checked;
				 
				 //preprocessing_data.start(read_csv.raw_data,read_csv.attribute_index,read_csv.time_index,trackBar1->Value);
				 //System::Windows::Forms::MessageBox::Show(preprocessing_data.num_of_five_minutes.ToString() + " " + histogram_position_table.size());
				 histogram->resize();
				 //System::Windows::Forms::MessageBox::Show(preprocessing_data.num_of_five_minutes.ToString() + " " + histogram_position_table.size());
			 }

	private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) 
			 {
				 textBox1->Text = "" + trackBar1->Value;
				 //System::Windows::Forms::MessageBox::Show(trackBar1->Value.ToString() + " " + trackBar1->TickFrequency.ToString() );		
			 }

	private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
			 {
			 }

	private: System::Void panel3_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 }

	private: System::Void panel3_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
				 this->panel3->Focus();
			 }
	private: System::Void panel1_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
				 this->panel1->Focus();
			 }
	private: System::Void panel2_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
				 this->panel2->Focus();
			 }

	private: System::Void panel3_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 Move_3_21_flag = false;
				 Move_12_horiz_flag = false;
				 Move_12_vert_flag = false;
			}
	private: System::Void panel1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 Move_3_21_flag = false;
				 Move_12_horiz_flag = false;
				 Move_12_vert_flag = false;
			 }
	private: System::Void panel2_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 Move_3_21_flag = false;
				 Move_12_horiz_flag = false;
				 Move_12_vert_flag = false;
				
			 }

	private: System::Void Form1_MouseMove_1(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
					 ///////////////move panel3 vs panel12 horizontally///////////////
					 int side_3_21_x1 = panel3->Location.X + panel3->Width;
					 int side_3_21_x2 = panel1->Location.X;
					 if(e->X > side_3_21_x1 && e->X < side_3_21_x2)
					 {
						 Move_3_21_flag = true;
						 this->Cursor = System::Windows::Forms::Cursors::NoMoveHoriz;
					 }

					 int move_3_21_x = e->X - last_x;
					 if(Down_3_21_flag)
					 {
						panel3->Width += move_3_21_x;
						panel1->Location = System::Drawing::Point(panel1->Location.X+move_3_21_x, panel1->Location.Y);
						panel1->Width -= move_3_21_x;
						panel2->Location = System::Drawing::Point(panel2->Location.X+move_3_21_x, panel2->Location.Y);
						panel2->Width -= move_3_21_x;	
					 }
					 ///////////////////move pane1 & panel2 horizontally//////////////////////
					 int side_12_x1 = panel2->Location.X + panel2->Width;
					 int side_12_x2 = side_12_x1 + 5;
					 int side_12_y1 = panel1->Location.Y;
					 int side_12_y2 = panel2->Location.Y + panel2->Height;
					 if(e->X > side_12_x1 && e->X < side_12_x2 && e->Y > side_12_y1 && e->Y < side_12_y2)
					 {
						 Move_12_horiz_flag = true;
						 this->Cursor = System::Windows::Forms::Cursors::NoMoveHoriz;							
					 }
					 else
					 {
						Move_12_horiz_flag = false;
					 }

					 int move_12_x = e->X - last_x;
					 if(Down_12_horiz_flag)
					 {
						 //panel2->Location = System::Drawing::Point(panel2->Location.X,panel2->Location.Y+move_12_y);
						 panel1->Width += move_12_x;
						 panel2->Width += move_12_x;
					 }
					 ///////////////move panel1 & panel2 vertically///////////////
					 side_12_x1 = panel1->Location.X;
					 side_12_x2 = panel1->Location.X + panel1->Width;
					 side_12_y1 = panel1->Location.Y;
					 side_12_y2 = panel2->Location.Y;
					 if(e->X > side_12_x1 && e->X < side_12_x2 && e->Y > side_12_y1 && e->Y < side_12_y2)
					 {
						 Move_12_vert_flag = true;
						 this->Cursor = System::Windows::Forms::Cursors::NoMoveVert;							
					 }
					 else
					 {
						Move_12_vert_flag = false;
					 }

					 int move_y = e->Y - last_y;
					 if(Down_12_vert_flag)
					 {
						 panel2->Location = System::Drawing::Point(panel2->Location.X,panel2->Location.Y+move_y);
						 panel1->Height += move_y;
						 panel2->Height -= move_y;
					 }
					 ////////////////////////////////////////////////////
					 last_x = e->X;
					 last_y = e->Y;
			 }


	private: System::Void Form1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
					last_x = e->X;
					last_y = e->Y;

					if(Move_3_21_flag)
					{
						Down_3_21_flag = true;
					}

					if(Move_12_horiz_flag)
					{
						Down_12_horiz_flag = true;
					}

					if(Move_12_vert_flag)
					{
						Down_12_vert_flag = true;
					}
			 }
	private: System::Void Form1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
					Down_3_21_flag = false;
					Down_12_horiz_flag = false;
					Down_12_vert_flag = false;
			 }

	private: System::Void Form1_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
					Form::Focus();
			 }

	private: System::Void residential_in_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				//preprocessing_data.Initial_selection_flag(this->residential_in->Checked,
				//										  this->residential_out->Checked,
				//										  this->work_school_in->Checked,
				//										  this->work_school_out->Checked,
				//										  this->tourism_in->Checked,
				//										  this->tourism_out->Checked);
			 }
	private: System::Void residential_out_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				//preprocessing_data.Initial_selection_flag(this->residential_in->Checked,
				//										  this->residential_out->Checked,
				//										  this->work_school_in->Checked,
				//										  this->work_school_out->Checked,
				//										  this->tourism_in->Checked,
				//										  this->tourism_out->Checked);
			 }
	private: System::Void work_school_in_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	//			preprocessing_data.Initial_selection_flag(this->residential_in->Checked,
	//													  this->residential_out->Checked,
	//													  this->work_school_in->Checked,
	//													  this->work_school_out->Checked,
	//													  this->tourism_in->Checked,
	//													  this->tourism_out->Checked);
			 }
	private: System::Void work_school_out_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				//preprocessing_data.Initial_selection_flag(this->residential_in->Checked,
				//										  this->residential_out->Checked,
				//										  this->work_school_in->Checked,
				//										  this->work_school_out->Checked,
				//										  this->tourism_in->Checked,
				//										  this->tourism_out->Checked);
			 }

	private: System::Void tourism_in_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				//preprocessing_data.Initial_selection_flag(this->residential_in->Checked,
				//										  this->residential_out->Checked,
				//										  this->work_school_in->Checked,
				//										  this->work_school_out->Checked,
				//										  this->tourism_in->Checked,
				//										  this->tourism_out->Checked);
			 }

	private: System::Void tourism_out_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				//preprocessing_data.Initial_selection_flag(this->residential_in->Checked,
				//										  this->residential_out->Checked,
				//										  this->work_school_in->Checked,
				//										  this->work_school_out->Checked,
				//										  this->tourism_in->Checked,
				//										  this->tourism_out->Checked);
			 }


	private: System::Void backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
				 //System::Windows::Forms::MessageBox::Show("Processing was completed");
			 }
	private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
				for (int i = 0; i < 100; i++)
				{
					//System::Threading::Thread::Sleep(10);
					backgroundWorker1->ReportProgress(i);
				}    
			 }

	private: System::Void panel2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 }
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->pictureBox1->Image = Image::FromFile("MRT_map.jpg");

			 }

	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 int selectedIndex = comboBox1->SelectedIndex;
				 Object^ selectedItem = comboBox1->SelectedItem;

				 preprocessing_data.comboBox_indx = selectedIndex;
				 //System::Windows::Forms::MessageBox::Show("Selected Item Text: " + selectedItem + "\n" + "Index: " + selectedIndex.ToString());
			 }
private: System::Void view_select_left_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 int selectedIndex = view_select_left->SelectedIndex;
				 Object^ selectedItem = view_select_left->SelectedItem;

				 preprocessing_data.view_select_left_index = selectedIndex;
		 }
private: System::Void view_select_right_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 int selectedIndex = view_select_right->SelectedIndex;
				 Object^ selectedItem = view_select_right->SelectedItem;

				 preprocessing_data.view_select_right_index = selectedIndex;
		 }
private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
				//this->pictureBox1->Image = Image::FromFile("MRT_map2.jpg");
					//ToolTip^ TP = gcnew ToolTip();
					//TP->ShowAlways = true;
					//TP->SetToolTip(textBox1, "Please input your name.");
		 }

private: System::Void panel1_MouseHover(System::Object^  sender, System::EventArgs^  e) {
					//ToolTip^ TP = gcnew ToolTip();
					//TP->ShowAlways = true;
					//TP->SetToolTip(panel1, "Please input your name.");
		 }
};
}

