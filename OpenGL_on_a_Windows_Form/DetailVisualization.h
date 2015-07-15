#pragma once

#include "VisualizationPanel.h"
#include <math.h>
#include <stdio.h>

namespace OpenGLForm{
	public ref class DetailVisualization: public OpenGLForm::VisualizationPanel{
		public:
			DetailVisualization(Form ^ parentForm,Panel ^ parentPanel, GLsizei iWidth, GLsizei iHeight,ReadCSV read_csv_ref,Preprocessing_Data preprocessing_data_ref);
			virtual System::Void Render(int,int) override;
			void  DetailMouseDown( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e);
			void  DetailMouseWheel( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e);
			void  DetailMouseMove( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e);
			void  DetailMouseUp( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e);
			System::Void clear();
	private:
			System::Void DrawText_FTGL(float n,int x, int y, int, int ,int);
			System::Void DrawText_FTGL_Zero_Padding(float n,int x, int y, int, int ,int);
			System::Void DrawTitle_FTGL(int t,int x, int y);
			System::Void DrawLine(int x1,int y1,int x2,int y2);
			System::Void title_string();
	};
}