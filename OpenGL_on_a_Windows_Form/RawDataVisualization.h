#pragma once


#include "VisualizationPanel.h"
namespace OpenGLForm{
	public ref class RawDataVisualization: public OpenGLForm::VisualizationPanel{
		public:
			RawDataVisualization(Form ^ parentForm,Panel ^ parentPanel, GLsizei iWidth, GLsizei iHeight,ReadCSV read_csv_ref,Preprocessing_Data preprocessing_data_ref);
			virtual System::Void Render(int,int) override;
			void  RawDataMouseDown( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e);
			void  RawDataMouseWheel( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e);
			void  RawDataMouseMove( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e);
			void  RawDataMouseUp( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e);
			void  RawDataMouseDoubleClick( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e);
			System::Void clear();
			System::Void clear_view();
			char* show_name;
			int* dim_label;
			//float test;
	private:
			System::Void FindPatternByTable2(int x,int y);
			vector3 Unprojection(vector2 _2Dpos);
			System::Void FindHistogram(int x,int y);
			System::Void DrawTime_FTGL(int index,int x, int y);
			System::Void DrawText_FTGL(int,int,int,float);
			System::Void DrawText_FTGL_withColor(int,int,int,float, float,float,float);
			//System::Void DrawCircle(int,int,float,float,float,float);
			System::Void time_string();
			System::Void DrawTitle_FTGL(int t,int x, int y);
			System::Void DrawTitle_FTGL_vertically(int t,int x, int y);
			System::Void DrawStationName(int, int, int, float);

			System::Void themeriverView();
			System::Void stationView();
			System::Void DrawVarianceColorBar();
			int drag, rect_x, rect_y;
			RECTANGLE* ROI;
			bool draw_roi;
			
			Line* temp_line;
			int line_draw; 
			int line_idx;
			System::Void FindPointInRect();
			System::Void FindStation(int x,int y);
			//System::Void DrawRect(int x, int y, int h, int w, vector<float>);

			//int* dim_label;
			Panel ^ parentPanel;

	};
}