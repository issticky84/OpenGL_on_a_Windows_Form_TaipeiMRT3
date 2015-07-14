#include "stdafx.h"
#include "DetailVisualization.h"

namespace OpenGLForm{
	DetailVisualization::DetailVisualization(Form ^ parentForm,Panel ^ parentPanel, GLsizei iWidth, GLsizei iHeight,ReadCSV read_csv_ref,Preprocessing_Data preprocessing_data_ref):VisualizationPanel(parentForm,parentPanel,iWidth,iHeight,read_csv_ref,preprocessing_data_ref){
			title_string();
			//Initialize mouse handler variable
			scale_x[2] = 0.0; scale_y[2] = 0.0; scale_z[2] = 0.0;
			scale_size[2] = 0.05;
			move_x[2] = 0.0; move_y[2] = 0.0; move_z[2] = 0.0;
			scale_factor[2] = 0.6;
			//Initialize window size
			windowWidth[2] = iWidth; 
			windowHeight[2] = iHeight;	
	}
	System::Void DetailVisualization::Render(int detail_width,int detial_height){
			wglmakecur();

			windowWidth[2] = detail_width;
			windowHeight[2] = detial_height;

			glClearColor(0.0, 0.0, 0.0, 0.0);  //Set the cleared screen colour to black
			glViewport(0, 0, windowWidth[2], windowHeight[2]);   //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window		
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, windowWidth[2], windowHeight[2], 0, -10, 10);

			//Back to the modelview so we can draw stuff 
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the screen and depth buffer

			glTranslatef(0.0+move_x[2],0.0+move_y[2],0.0+move_z[2]);
			glScalef(scale_factor[2]+scale_x[2],scale_factor[2]+scale_y[2],scale_factor[2]+scale_z[2]);


			vector<float> draw_color(3);
			draw_color[0] = 0.8; draw_color[1] = 1.0; draw_color[2] = 1.0;
			DrawRect(20, 430+40, 2.0, 100.0, draw_color);//橫線
			DrawTitle_FTGL(0, 130, 430+35); //enter
			DrawRect(20, 430-60, 100.0, 2.0, draw_color);//直線
			DrawTitle_FTGL(1, 10, 430-90); //out
			//=====================畫網格====================//
			//30格代表1000的值=>值*3/100=格的位置
			//30格代表500的值=>值*3/50=格的位置
			
			float x_position = 120;
			float y_position = 420;
			float value = 0.0;
			for(int i=0;i<=390;i+=30)
			{
				DrawText_FTGL(value, x_position + i, y_position + 10, 1.0, 1.0, 1.0);
				value += 0.5;
				//DrawRect(x_position, y_position - i, 2.0, 390.0, draw_color);
				//DrawLine(1.0, x_position + i, y_position, x_position + i, y_position - 390);
			}

			x_position = 120;
			y_position = 30;
			value = 6.5;
			for(int i=0;i<=390;i+=30)
			{
				if(value>0.001) DrawText_FTGL(value, x_position-20, y_position + i, 1.0, 1.0, 1.0);
				value -= 0.5;
				//DrawLine(1.0, x_position, y_position + i, x_position + 390, y_position + i);
				//DrawRect(x_position + i, y_position, 390.0, 2.0, draw_color);
			}
			
			//=============================================//
			if(preprocessing_data.view_select_right_index==2)
			{
				int t = 0;
				for(int i=0;i<preprocessing_data.sample_num_x;i++)
				{
					for(int j=0;j<preprocessing_data.sample_num_y;j++)
					{
						int px = i*preprocessing_data.sample_unit;
						int py = j*preprocessing_data.sample_unit;
						draw_color[0] = preprocessing_data.sample_color.at<float>(t,0); 
						draw_color[1] = preprocessing_data.sample_color.at<float>(t,1); 
						draw_color[2] = preprocessing_data.sample_color.at<float>(t,2);
						DrawRect(120.0 + px*(3.0/50.0), 420.0 - py*(3.0/50.0), preprocessing_data.sample_unit*(3.0/50.0), preprocessing_data.sample_unit*(3.0/50.0), draw_color);
						//DrawCircle(120 + px*(3.0/50.0), 420.0 - py*(3.0/50.0), 4.0, 
						//	   preprocessing_data.sample_color.at<float>(t,0), preprocessing_data.sample_color.at<float>(t,1), preprocessing_data.sample_color.at<float>(t,2));
						t++;
					}
				}
			}
			////////////
			for(int i=0;i<preprocessing_data.cluster_center_raw.rows;i++)
			{
				//DrawCircle(120.0 + preprocessing_data.cluster_center_raw.at<float>(i,0)*(3.0/50.0), 420.0 - preprocessing_data.cluster_center_raw.at<float>(i,1)*(3.0/50.0), 4.0, 
				//		   preprocessing_data.rgb_mat3.at<float>(i,0), preprocessing_data.rgb_mat3.at<float>(i,1), preprocessing_data.rgb_mat3.at<float>(i,2));
				drawHallowCircle(120.0 + preprocessing_data.cluster_center_raw.at<float>(i,0)*(3.0/50.0), 420.0 - preprocessing_data.cluster_center_raw.at<float>(i,1)*(3.0/50.0), 4.0, 
						   preprocessing_data.rgb_mat3.at<float>(i,0), preprocessing_data.rgb_mat3.at<float>(i,1), preprocessing_data.rgb_mat3.at<float>(i,2));
			}


			SwapOpenGLBuffers();
			
		}
		 
		System::Void DetailVisualization::DrawText_FTGL(float n,int x, int y, int r, int g, int b)
		{
			glPushMatrix();

			float font_size = 8*(scale_factor[2]+0.6+scale_x[2]);
			font.FaceSize(font_size);
			glColor3f(r, g, b);
			glRasterPos2f(x , y + font.LineHeight());
			stringstream ss;
			ss << n;
			string str = ss.str();
			text = (char*)str.c_str();
			font.Render(text);

			glPopMatrix();
		}

		System::Void DetailVisualization::DrawText_FTGL_Zero_Padding(float n,int x, int y, int r, int g, int b)
		{
			glPushMatrix();

			float font_size = 10*(scale_factor[2]+0.6+scale_x[2]);
			font.FaceSize(font_size);
			glColor3f(r, g, b);
			glRasterPos2f(x , y + font.LineHeight());
			stringstream ss;
			ss << n;
			string str = ss.str();
			char zero[] = "0";
			if(n<10)
			{
				strcat(zero,str.c_str());
				font.Render(zero);
			}
			else
			{
				text = (char*)str.c_str();
				font.Render(text);
			}
			glPopMatrix();
		}

		System::Void DetailVisualization::DrawTitle_FTGL(int t,int x, int y)
		{
			strcpy(title[0],"Enter (K)");
			strcpy(title[1],"Out (K)");

			glPushMatrix();

			float font_size = 16*(scale_factor[2]+0.4+scale_x[2]);
			font.FaceSize(font_size);
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(x , y + font.LineHeight());
			font.Render(title[t]);

			glPopMatrix();			
		}

		System::Void DetailVisualization::title_string()
		{
			//strcpy(title[0],"住宅區 (進站)");
			//strcpy(title[1],"住宅區 (出站)");
			//strcpy(title[2],"辦公/學校 (進站)");
			//strcpy(title[3],"辦公/學校 (出站)");
			//strcpy(title[4],"觀光區 (進站)");
			//strcpy(title[5],"觀光區 (出站)");
			//strcpy(title[0],"Residential (In)");
			//strcpy(title[1],"Residential (Out)");
			//strcpy(title[2],"Commercial/School (In)");
			//strcpy(title[3],"Commercial/School (Out)");
			//strcpy(title[4],"Tourism (In)");
			//strcpy(title[5],"Tourism (Out)");
		}

		System::Void DetailVisualization::DrawLine(int x1,int y1,int x2,int y2)
		{
			glPushMatrix();
			glLineWidth(2.0);
			glBegin(GL_LINES);
				glColor3f(1.0,1.0,1.0);
				glVertex3f(x1,y1,0.0);
				glVertex3f(x2,y2,0.0);
			glEnd();
			glPopMatrix();	
		}

		System::Void DetailVisualization::clear()
		{
			//raw_data_index.clear();
			raw_data_index_2D.clear();
		}
}