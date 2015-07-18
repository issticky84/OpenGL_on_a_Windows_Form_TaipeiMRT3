#include "stdafx.h"
#include "RawDataVisualization.h"
#include <fstream>
#include <cstdio>

namespace OpenGLForm{

	RawDataVisualization::RawDataVisualization(Form ^ parentForm,Panel ^ parentPanel, GLsizei iWidth, GLsizei iHeight,ReadCSV read_csv_ref,Preprocessing_Data preprocessing_data_ref):VisualizationPanel(parentForm,parentPanel,iWidth,iHeight,read_csv_ref,preprocessing_data_ref){
			parentPanel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &RawDataVisualization::RawDataMouseDown);
			parentPanel->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler( this, &RawDataVisualization::RawDataMouseWheel );
			parentPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler( this, &RawDataVisualization::RawDataMouseMove );	
			parentPanel->MouseUp += gcnew System::Windows::Forms::MouseEventHandler( this, &RawDataVisualization::RawDataMouseUp );	
			parentPanel->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler( this, &RawDataVisualization::RawDataMouseDoubleClick );	 
			this->parentPanel = parentPanel;
			time_string();
			//Initialize mouse handler variable
			scale_x[1] = 0.5; scale_y[1] = 0.5; scale_z[1] = 0.0;
			scale_size[1] = 0.005;
			move_x[1] = 0.0; move_y[1] = 0.0; move_z[1] = 0.0;
			scale_factor[1] = 0.0;
			//Initialize window size
			windowWidth[1] = iWidth; 
			windowHeight[1] = iHeight;
			//raw_data_position_table.resize(100);
			//for(int i=0;i<100;i++) raw_data_position_table[i].resize(31);
			//raw_data_position_table.resize( preprocessing_data.month_vec.size() );
			//for(int i=0;i<preprocessing_data.month_vec.size();i++) raw_data_position_table[i].resize( preprocessing_data.month_vec[i].day_vec.size() );
			select_histogram_flag = false;

			drag = 0;
			ROI = new RECTANGLE;
			draw_roi = false;

			dim_label = new int[199];
			for(int i=0;i<199;i++) dim_label[ i ] = 0;

			int dim_index[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88,29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,
								35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77,22,7,19,
								24,8,11,16,41,40,39,36,130,97,82,80,42,131,93,92,86,27,65,61,58,57,56,33,85,71,68,94,9};			
			int station_num = sizeof(dim_index)/sizeof(dim_index[0]);
			int home[] = {29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77};//43
			int home_num = sizeof(home)/sizeof(home[0]);
			int work_school[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88};//20
			int work_school_num = sizeof(work_school)/sizeof(work_school[0]);
			for(int i=0;i<station_num;i++) dim_label[ dim_index[i] ] = 1;
			for(int i=0;i<home_num;i++)  dim_label[ home[i] ] = 2;
			for(int i=0;i<work_school_num;i++)  dim_label[ work_school[i] ] = 3;

			temp_line = new Line;

	}

	System::Void RawDataVisualization::themeriverView()
	{
			glTranslatef(35.0+move_x[1],190.0+move_y[1],0.0+move_z[1]);
			glScalef(scale_factor[1]+scale_x[1]+0.6, scale_factor[1]+scale_y[1]-0.480, scale_factor[1]+scale_z[1]);
			glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewMatrix2);//////////////

			if(!histogram_index.empty())
			{
				int y_position = 0;
				int y_position_text = 0;
				for(int u=0;u<histogram_index.size();u++)
				{
					int idx = histogram_index[u];
					int this_month = preprocessing_data.find_month_and_day[idx].at<int>(0,0);
					int this_day = preprocessing_data.find_month_and_day[idx].at<int>(0,1);

					DrawText_FTGL(this_month+1,-20,y_position_text,16.0);
					DrawText_FTGL(this_day+1,-5,y_position_text,16.0);
					y_position_text+=10000;

					//畫折線圖的刻度:1000
					vector<float> draw_color(3);
					draw_color[0] = 0.8; draw_color[1] = 1.0; draw_color[2] = 1.0;
					DrawText_FTGL(1000,15,y_position-1000,10.0);
					DrawRect(40, y_position-1000, 50.0, 630.0, draw_color);
					//畫折線圖的刻度:2000
					DrawText_FTGL(2000,15,y_position-2000,10.0);
					DrawRect(40, y_position-2000, 50.0, 630.0, draw_color);
					//畫折線圖的刻度:3000
					DrawText_FTGL(3000,15,y_position-3000,10.0);
					DrawRect(40, y_position-3000, 50.0, 630.0, draw_color);
					//畫折線圖的刻度:4000
					DrawText_FTGL(4000,15,y_position-4000,10.0);
					DrawRect(40, y_position-4000, 50.0, 630.0, draw_color);
					//畫折線圖的刻度:5000
					DrawText_FTGL(5000,15,y_position-5000,10.0);
					DrawRect(40, y_position-5000, 50.0, 630.0, draw_color);

					for(int i=5;i<=23;i++)
					{
						//int color_index = preprocessing_data.color_num - 1;
	
						int tag = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].cluster_tag;


						vector<float> draw_color(3);
						draw_color[0] = preprocessing_data.rgb_mat3.at<float>(tag,0);
						draw_color[1] = preprocessing_data.rgb_mat3.at<float>(tag,1);
						draw_color[2] = preprocessing_data.rgb_mat3.at<float>(tag,2);
						//畫數字(小時)
						DrawRect(i*15-5, y_position+1350, 650.0, 15.0, draw_color);

						DrawText_FTGL_withColor(i,i*15-5,y_position+1000,12.0, 1.0, 1.0, 1.0);

						//畫折線圖
						for(int j=0; j<preprocessing_data.select_station.size() && i!=23 ;j++)
						//for(int j=preprocessing_data.select_station.size()-1; j>0 && i!=23 ;j--)
						{
							int hour_data_current = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].enter[ preprocessing_data.select_station[j] ];
							int hour_data_next = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].enter[ preprocessing_data.select_station[j] ];

							glLineWidth(3.0); 

							glPushMatrix(); 
							glBegin(GL_LINES); 
							glColor3f(preprocessing_data.data_color[j][0],preprocessing_data.data_color[j][1],preprocessing_data.data_color[j][2]);  
								glVertex3f((i+1)*15,y_position-hour_data_next,0); 
								glVertex3f(i*15,y_position-hour_data_current,0);						
							glEnd();
							glPopMatrix();

							//color_index--;
						}

						//out
						//color_index = preprocessing_data.color_num - 1;

						draw_color[0] = preprocessing_data.rgb_mat3.at<float>(tag,0);
						draw_color[1] = preprocessing_data.rgb_mat3.at<float>(tag,1);
						draw_color[2] = preprocessing_data.rgb_mat3.at<float>(tag,2);
						DrawRect(300+i*15-5, y_position+1350, 650.0, 15.0, draw_color);
						DrawText_FTGL_withColor(i,300+i*15-5,y_position+1000,12.0, 1.0, 1.0, 1.0);

						for(int j=0; j<preprocessing_data.select_station.size() && i!=23 ;j++)
						//for(int j=preprocessing_data.select_station.size()-1 ; j>0 && i!=23 ;j--)
						{
							int hour_data_current = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].out[ preprocessing_data.select_station[j] ];
							int hour_data_next = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].out[ preprocessing_data.select_station[j] ];

							glLineWidth(3.0); 

							glPushMatrix(); 
							glBegin(GL_LINES); 
							glColor3f(preprocessing_data.data_color[j][0],preprocessing_data.data_color[j][1],preprocessing_data.data_color[j][2]);  
								glVertex3f(300+(i+1)*15,y_position-hour_data_next,0); 
								glVertex3f(300+i*15,y_position-hour_data_current,0);						
							glEnd();
							glPopMatrix();

							//color_index--;
						}
					}

					//DrawText_FTGL(23,23*15-5,y_position+750,12.0);
					//DrawText_FTGL(23,300+23*15-5,y_position+750,12.0);

					y_position+= 10000;
				}
			}	
	}

	System::Void RawDataVisualization::stationView()
	{
			scale_x[1] = 0.5; scale_y[1] = 0.5; scale_z[1] = 0.0;
			move_x[1] = 0.0; move_y[1] = 0.0; move_z[1] = 0.0;
			//glTranslatef(0.0+move_x[1],0.0+move_y[1],0.0+move_z[1]);
			//glScalef(0.5+scale_factor[1]+scale_x[1], 0.5+scale_factor[1]+scale_y[1], 1.0+scale_factor[1]+scale_z[1]);
			glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewMatrix2);		

			int dim_index[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88,29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,
								35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77,22,7,19,
								24,8,11,16,41,40,39,36,130,97,82,80,42,131,93,92,86,27,65,61,58,57,56,33,85,71,68,94,9};	

			//if(preprocessing_data.day_select_right_indx == 0)
			//{
				//DrawTitle_FTGL(0,50,50); //draw text "weekday"
				for(int i=0; i<preprocessing_data.dim ;i++)
				{	
					int dim_num = dim_index[i];

					vector<float> color(3);
				
					if(dim_label[dim_num]==4)
					{
						color[0] = preprocessing_data.station_color_mat.at<float>(i,0);
						color[1] = preprocessing_data.station_color_mat.at<float>(i,1);
						color[2] = preprocessing_data.station_color_mat.at<float>(i,2);
					}
					else if(!preprocessing_data.select_station.empty() )
					{
						color[0] = 0.3;
						color[1] = 0.3;
						color[2] = 0.3;
					}
					else
					{
						color[0] = preprocessing_data.station_color_mat.at<float>(i,0);
						color[1] = preprocessing_data.station_color_mat.at<float>(i,1);
						color[2] = preprocessing_data.station_color_mat.at<float>(i,2);			
					}
				

					DrawCircle(80 + 1.5*preprocessing_data.position_on_2D.at<double>(i,0), 70 + 1.2*preprocessing_data.position_on_2D.at<double>(i,1), 4.0, color[0], color[1], color[2]);
					/*
					glPointSize( 8.0 );

					glPushMatrix();
					glBegin( GL_POINTS );
				
					if(dim_label[dim_num]==4)
					{
						glColor3f(preprocessing_data.station_color_mat_weekday.at<float>(i,0), 
								  preprocessing_data.station_color_mat_weekday.at<float>(i,1),
								  preprocessing_data.station_color_mat_weekday.at<float>(i,2));
					}
						//glColor3f( 0.8f, 0.0, 0.0f );
					else if(!preprocessing_data.select_station.empty() )
					{
						glColor3f( 0.3f, 0.3, 0.3f );
					}
					else
					{
						glColor3f(preprocessing_data.station_color_mat_weekday.at<float>(i,0), 
								  preprocessing_data.station_color_mat_weekday.at<float>(i,1),
								  preprocessing_data.station_color_mat_weekday.at<float>(i,2));				
					}


					glVertex2f(80 + 1.5*preprocessing_data.position_on_2D.at<double>(i,0), 70 + 1.2*preprocessing_data.position_on_2D.at<double>(i,1) );
					glEnd();
					glPopMatrix();
					*/
					//DrawCircle(preprocessing_data.position_on_2D.at<double>(day,0),preprocessing_data.position_on_2D.at<double>(day,1), 2.0, 1, 0, 0);
					//DrawCircle(preprocessing_data.position_on_2D.at<double>(day,0),preprocessing_data.position_on_2D.at<double>(day,1), 2.0, (rand() % 11)/10.0, (rand() % 11)/10.0, (rand() % 11)/10.0);
					//DrawCircle(preprocessing_data.position_on_2D.at<double>(i,0),preprocessing_data.position_on_2D.at<double>(i,1), 2.0, preprocessing_data.station_color_mat_weekday.at<float>(i,0), 
					//																													  preprocessing_data.station_color_mat_weekday.at<float>(i,1), 
					//																													  preprocessing_data.station_color_mat_weekday.at<float>(i,2));
				}	
			//}
			//else if(preprocessing_data.day_select_right_indx == 1)
			//{
				/*
				DrawTitle_FTGL(1,50,560);//draw text "weekend"
				for(int i=0; i<preprocessing_data.dim ;i++)
				{	
					int dim_num = dim_index[i];
				
					vector<float> color(3);

					//glPointSize( 8.0 );

					//glPushMatrix();
					//glBegin( GL_POINTS );
				
					if(dim_label[dim_num]==4)
					{
						color[0] = preprocessing_data.station_color_mat_weekend.at<float>(i,0);
						color[1] = preprocessing_data.station_color_mat_weekend.at<float>(i,1);
						color[2] = preprocessing_data.station_color_mat_weekend.at<float>(i,2);
					}
						//glColor3f( 0.8f, 0.0, 0.0f );
					else if(!preprocessing_data.select_station.empty() )
					{
						color[0] = 0.3;
						color[1] = 0.3;
						color[2] = 0.3;
					}
					else
					{
						color[0] = preprocessing_data.station_color_mat_weekend.at<float>(i,0);
						color[1] = preprocessing_data.station_color_mat_weekend.at<float>(i,1);
						color[2] = preprocessing_data.station_color_mat_weekend.at<float>(i,2);			
					}
				

					//glVertex2f(80 + 1.5*preprocessing_data.position_on_2D.at<double>(i,0), 550 + 1.2*preprocessing_data.position_on_2D.at<double>(i,1) );
				
					//glEnd();
					//glPopMatrix();

					DrawCircle(80 + 1.5*preprocessing_data.position_on_2D.at<double>(i,0), 560 + 1.2*preprocessing_data.position_on_2D.at<double>(i,1), 4.0, color[0], color[1], color[2]);
				}	
				*/
			//}
	}

	System::Void RawDataVisualization::Render(int rawdata_width,int rawdata_height){
			wglmakecur();
				
			windowWidth[1] = rawdata_width;
			windowHeight[1] = rawdata_height;
		   //////////////////////////
		   GLfloat values[2];
		   glGetFloatv (GL_LINE_WIDTH_GRANULARITY, values);
		   printf ("GL_LINE_WIDTH_GRANULARITY value is %3.1f\n",
			  values[0]);
		   glGetFloatv (GL_LINE_WIDTH_RANGE, values);
		   printf ("GL_LINE_WIDTH_RANGE values are %3.1f %3.1f\n",
			  values[0], values[1]);

		   glEnable (GL_LINE_SMOOTH);
		   glEnable (GL_BLEND);
		   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		   glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		   //glLineWidth (1.5);
		   /////////////////////////
			glClearColor(0.0, 0.0, 0.0, 0.0);  //Set the cleared screen colour to black
			glViewport(0, 0, windowWidth[1], windowHeight[1]);   //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window		
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, windowWidth[1], windowHeight[1], 0, -10, 10);

			//Back to the modelview so we can draw stuff 
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the screen and depth buffer

			if(preprocessing_data.view_select_right_index==2)
			{

				float x_position = 30;
				float y_position = 30;
				vector<float> color(3);
				for(int i=0;i<preprocessing_data.select_station.size();i++)
				{
					color[0] = preprocessing_data.data_color[i][0];
					color[1] = preprocessing_data.data_color[i][1];
					color[2] = preprocessing_data.data_color[i][2];
					DrawCircle(x_position-5, y_position, 3.0, color[0], color[1], color[2]);
					int wchar_len = DrawTextOfStationName(preprocessing_data.select_station[i], x_position, y_position, 20);
					//x_position += 150;
					if(wchar_len==7) //有括號的間距可以小一點(5個字+2括號)
					{
						x_position += (wchar_len-2)*30;
					}
					else
					{
						x_position += wchar_len*30;
					}
					if(x_position > 700)
					{
						y_position += 25;
						x_position = 30;
					}
				}
				/*
				glPushMatrix();

				font_new->FaceSize(20);
				//glScalef(1.0+scale_x[1], 1.0+scale_y[1], 1.0);
				glTranslatef(50, 50, 0);
				//glRotatef(270, 0.0, 0.0, -1.0);
				glRotatef(180, 1.0, 0.0, 0.0);
				//glRotatef(test++, 1.0, 0.0, 0.0);
				wchar_t strText[] = L"松山機場"; 
				glColor3f(1.0, 1.0, 1.0);
				font_new->Render(strText);
				//font_new->Render(station_name[7]);

				glPopMatrix();
				*/

				themeriverView();
				//DrawTextOfStationName(8, 100, 100, 1.0);
				//DrawStationName(7, 500, 500, 2.0);
				//DrawTextOfStationName(7, 500, 500, 2.0);
			}
			else if(preprocessing_data.view_select_right_index==1)
			{
				stationView();

				DrawLine(1.5, temp_line->x1, temp_line->y1, temp_line->x2, temp_line->y1);
				DrawLine(1.5, temp_line->x1, temp_line->y1, temp_line->x1, temp_line->y2);
				DrawLine(1.5, temp_line->x1, temp_line->y2, temp_line->x2, temp_line->y2);
				DrawLine(1.5, temp_line->x2, temp_line->y1, temp_line->x2, temp_line->y2);
	
				DrawVarianceColorBar();
			}

			SwapOpenGLBuffers();
			
	}
		 
	System::Void RawDataVisualization::DrawVarianceColorBar()
	{
		//=================Variance Color Bar================//
		/*
		float x_position = 200;
		float y_position = 470;
		float width = 10.0;
		DrawTitle_FTGL(2, x_position-100, y_position-30);//draw text "Variance"
		DrawTitle_FTGL(3, x_position, y_position+20);//draw text "Small"
		for(int i=0;i<preprocessing_data.cov_color_bar_weekday.rows;i++)
		{
			vector<float> draw_color(3);
			draw_color[0] = preprocessing_data.cov_color_bar_weekday.at<float>(i,0);
			draw_color[1] = preprocessing_data.cov_color_bar_weekday.at<float>(i,1);
			draw_color[2] = preprocessing_data.cov_color_bar_weekday.at<float>(i,2);
			DrawRect(x_position, y_position, 15.0, width, draw_color);
			x_position += width;
		}
		DrawTitle_FTGL(4, x_position, y_position+20);//draw text "Large"
		///////////////////////////////////
		x_position = 200;
		y_position = 920;
		DrawTitle_FTGL(5, x_position-100, y_position-30);//draw text "Variance"
		DrawTitle_FTGL(3, x_position, y_position+20);//draw text "Small"
		for(int i=0;i<preprocessing_data.cov_color_bar_weekday.rows;i++)
		{
			vector<float> draw_color(3);
			draw_color[0] = preprocessing_data.cov_color_bar_weekend.at<float>(i,0);
			draw_color[1] = preprocessing_data.cov_color_bar_weekend.at<float>(i,1);
			draw_color[2] = preprocessing_data.cov_color_bar_weekend.at<float>(i,2);
			DrawRect(x_position, y_position, 15.0, width, draw_color);
			x_position += width;
		}
		DrawTitle_FTGL(4, x_position, y_position+20);//draw text "Large"	
		*/
		int t = 0;
		float x_position = 600;
		float y_position = 550;
		DrawTitle_FTGL(4, x_position-50, y_position+10);//small
		DrawTitle_FTGL(3, x_position+15, y_position+10);//weekend variance
		//DrawTitle_FTGL(2, x_position-140, y_position-50);//weekday variance
		DrawTitle_FTGL_vertically(2, x_position-15, y_position-10);//weekday variance
		for(int i=0;i<sqrt(preprocessing_data.cov_color_bar.rows);i++)
		{
			x_position = 600;
			for(int j=0;j<sqrt(preprocessing_data.cov_color_bar.rows);j++)
			{
				vector<float> draw_color(3);
				draw_color[0] = preprocessing_data.cov_color_bar.at<float>(t,0);
				draw_color[1] = preprocessing_data.cov_color_bar.at<float>(t,1);
				draw_color[2] = preprocessing_data.cov_color_bar.at<float>(t,2);
				t++;
				DrawRect(x_position, y_position, 4.0, 4.0, draw_color);		
				x_position += 4.0;
			}
			y_position -= 4.0;
		}
		DrawTitle_FTGL(5, x_position+10, y_position-30);//large
	}

	System::Void RawDataVisualization::FindPointInRect()
	{
		vector2 pos1(temp_line->x1,temp_line->y1);
		//pos1.x *= (scale_factor[1] + scale_x[1]);
		//pos1.y *= (scale_factor[1] + scale_y[1]);
		pos1.x += move_x[1];
		pos1.y += move_y[1];	
		vector2 pos2(temp_line->x2,temp_line->y2);
		//pos2.x *= (scale_factor[1] + scale_x[1]);
		//pos2.y *= (scale_factor[1] + scale_y[1]);
		pos2.x += move_x[1];
		pos2.y += move_y[1];	

		//Weekday
		for(int day=0; day<preprocessing_data.dim ;day++)
		{
			int x = 80 + 1.5*preprocessing_data.position_on_2D.at<double>(day,0);
			int y = 70 + 1.2*preprocessing_data.position_on_2D.at<double>(day,1);
			if(x >= pos1.x && x <= pos2.x && y >= pos1.y && y <= pos2.y)
			{
				int dim_index[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88,29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,
								   35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77,22,7,19,
								   24,8,11,16,41,40,39,36,130,97,82,80,42,131,93,92,86,27,65,61,58,57,56,33,85,71,68,94,9};

				dim_label[ dim_index[day] ] = 4;

				if ( !preprocessing_data.check_duplicated_station( dim_index[day]) )
					preprocessing_data.select_station.push_back( dim_index[day] );		

				preprocessing_data.comboBox_indx = 1;

			}
		}

		//Weekend
		/*
		for(int day=0; day<preprocessing_data.dim ;day++)
		{
			int x = 80 + 1.5*preprocessing_data.position_on_2D.at<double>(day,0);
			int y = 560 + 1.2*preprocessing_data.position_on_2D.at<double>(day,1);
			if(x >= pos1.x && x <= pos2.x && y >= pos1.y && y <= pos2.y)
			{
				int dim_index[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88,29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,
								   35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77,22,7,19,
								   24,8,11,16,41,40,39,36,130,97,82,80,42,131,93,92,86,27,65,61,58,57,56,33,85,71,68,94,9};

				dim_label[ dim_index[day] ] = 4;

				if ( !preprocessing_data.check_duplicated_station( dim_index[day]) )
					preprocessing_data.select_station.push_back( dim_index[day] );		

				preprocessing_data.comboBox_indx = 2;
			}
		}*/
	}

	System::Void RawDataVisualization::FindStation(int x,int y){
		vector2 pos_2D(x,y);
		vector3 pos_3D = Unprojection(pos_2D);//screen to 3D coordinate
		///////////bug///////////
		pos_3D.x *= (scale_factor[1] + scale_x[1]);
		pos_3D.y *= (scale_factor[1] + scale_y[1]);
		pos_3D.x += move_x[1];
		pos_3D.y += move_y[1];	
		//System::Windows::Forms::MessageBox::Show( pos_3D.x.ToString() + " " + pos_3D.y.ToString());	
		for(int day=0; day<preprocessing_data.dim ;day++)
		{
			int x = 80 + 1.5*preprocessing_data.position_on_2D.at<double>(day,0);
			int y = 70 + 1.2*preprocessing_data.position_on_2D.at<double>(day,1);
			x *= (scale_factor[1] + scale_x[1]);
			y *= (scale_factor[1] + scale_y[1]);
			x += move_x[1];
			y += move_y[1];

			//if( abs(pos_3D.x-x) <= 1.0 && abs(pos_3D.y-y) <= 1.0 )
			if( (pos_3D.x-x) <= 2.3 && (pos_3D.x-x) >=0 &&  (pos_3D.y-y) <= 2.3 && (pos_3D.y-y) >=0 )
			{
				int dim_index[] = {31,98,30,23,90,10,12,129,55,54,
					               53,52,51,50,42,132,91,89,133,88,
								   29,28,26,25,21,13,14,15,17,18,
								   70,69,66,64,63,62,60,59,43,38,
								   37,35,34,32,174,175,176,177,178,128,
								   45,46,47,48,96,95,85,84,83,81,
								   79,78,77,22,7,19,24,8,11,16,
								   41,40,39,36,130,97,82,80,42,131,
								   93,92,86,27,65,61,58,57,56,33,
								   85,71,68,94,9};

				char* station_name[] = {"","","","","","","","松山機場","中山國中","南京東路","忠孝復興(文湖線)",
										   "大安","科技大樓","六張犁","麟光","辛亥","萬芳醫院","萬芳社區","木柵","動物園","",
				                           "大直","劍南路","西湖","港墘","文德","內湖","大湖公園","葫洲","東湖","南港軟體園區",
				                           "南港展覽館(文湖線)","小碧潭","新店","新店區公所","七張","大坪林","景美","萬隆","公館","台電大樓",
				                           "古亭","中正紀念堂","小南門","","頂溪","永安市場","景安","南勢角","","臺大醫院",
				                           "台北車站(板南線)","台北車站(淡水線)","中山","雙連","民權西路(淡水線)","圓山","劍潭","士林","芝山","明德",
										   "石牌","唭哩岸","奇岩","北投","新北投","復興崗","忠義","關渡","竹圍","紅樹林",
										   "淡水","","","","","","永寧","土城","海山","亞東醫院",
										   "府中","板橋","新埔","江子翠","龍山寺","西門","","善導寺","忠孝新生(板南線)","忠孝復興(板南線)",
										   "忠孝敦化","國父紀念館","市政府","永春","後山埤","昆陽","南港","南港展覽館(板南線)","","",
										   "","","","","","","","","","",
										   "","","","","","","","","","",
										   "","","","","","","","大橋頭","民權西路(蘆洲線)","中山國小",
										   "行天宮","松江南京","忠孝新生(蘆洲線)","","","","","","","",
										   "","","","","","","","","","",
										   "","","","","","","","","","",
										   "","","","","","","","","","",
										   "","","","蘆洲","三民高中","徐匯中學","三和國中","三重國小","",""};
				

				System::String^ name= gcnew System::String( station_name[ dim_index[day] ]);
				System::Windows::Forms::MessageBox::Show( name + " " );
				
				//show_name = new char[150];
				//strcpy(show_name,station_name[ dim_index[day]]);
				/*
				ToolTip^ TP = gcnew ToolTip();
				TP->ShowAlways = true;
				TP->ReshowDelay = 500;
				//System::Windows::Forms::MessageBox::Show( name + " " );
				TP->SetToolTip(parentPanel, name);
				TP->AutoPopDelay = 5000;
				delete TP;
				*/
			}
		}	
	
	}

	System::Void RawDataVisualization::FindPatternByTable2(int x,int y){
		vector2 pos_2D(x,y);
		vector3 pos_3D = Unprojection(pos_2D);//screen to 3D coordinate
		///////////bug///////////
		//pos_3D.x *= (scale_factor[1] + scale_x[1]);
		//pos_3D.y *= (scale_factor[1] + scale_y[1]);
		//pos_3D.x += move_x[1];
		//pos_3D.y += move_y[1];	
		//System::Windows::Forms::MessageBox::Show( pos_3D.x.ToString() + " " + pos_3D.y.ToString());	
		for(int day=0; day<preprocessing_data.dim ;day++)
		{
			int x = 80 + 1.5*preprocessing_data.position_on_2D.at<double>(day,0);
			int y = 70 + 1.2*preprocessing_data.position_on_2D.at<double>(day,1);
			//x *= (scale_factor[1] + scale_x[1]);
			//y *= (scale_factor[1] + scale_y[1]);
			//x += move_x[1];
			//y += move_y[1];

			if( sqrt( (pos_3D.x-x)*(pos_3D.x-x) + (pos_3D.y-y)*(pos_3D.y-y) ) <= 4.0 )
			//if( abs(pos_3D.x-x) + abs(pos_3D.y-y) <= 4.0 )
			//if( (pos_3D.x-x) <= 2.3 && (pos_3D.x-x) >=0 &&  (pos_3D.y-y) <= 2.3 && (pos_3D.y-y) >=0 )
			{
				int dim_index[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88,29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,
								   35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77,22,7,19,
								   24,8,11,16,41,40,39,36,130,97,82,80,42,131,93,92,86,27,65,61,58,57,56,33,85,71,68,94,9};
				

				//System::Windows::Forms::MessageBox::Show( dim_index[day] + " ");

				dim_label[ dim_index[day] ] = 4;
				//if(dim_label[ dim_index[day] ] != 4) dim_label[ dim_index[day] ] = 4;
				//else if(dim_label[ dim_index[day] ] == 4) dim_label[ dim_index[day] ] = 1;

				if ( !preprocessing_data.check_duplicated_station( dim_index[day]) )
					preprocessing_data.select_station.push_back( dim_index[day] );
			}
		}

	}

	System::Void RawDataVisualization::FindHistogram(int x,int y)
	{
		vector2 pos_2D(x,y);
		vector3 pos_3D = Unprojection(pos_2D);//screen to 3D coordinate
		///////////bug///////////
		pos_3D.x *= (scale_factor[1] + scale_x[1]);
		pos_3D.y *= (scale_factor[1] + scale_y[1]);
		pos_3D.x += move_x[1];
		pos_3D.y += move_y[1];		
		for(int i=0;i<raw_data_position_table.size();i++)
		{
			if(histogram_index[i] > read_csv.raw_data_size-1) break; 
			for(int j=0;j<raw_data_position_table[i].size();j++)
			{
				if(histogram_index[i]+j > read_csv.raw_data_size-1) break;
				if(pos_3D.x >= raw_data_position_table[i][j].x && pos_3D.x <= raw_data_position_table[i][j].z && pos_3D.y >= raw_data_position_table[i][j].y && pos_3D.y <= raw_data_position_table[i][j].w)
				{
					select_histogram_index = histogram_index[i]/600;
					select_histogram_flag = true;
				}
			}
		}		
	}

	System::Void RawDataVisualization::DrawText_FTGL(int n,int x, int y, float scale)
	{
		glPushMatrix();

		glTranslatef(x, y-30, 0);
		glScalef(1.0+scale_x[1],1.0+scale_y[1],1.0+scale_z[1]);	
	
		//float font_size = 10*(scale_factor[2]+0.4+scale_x[2]);	
		font.FaceSize(scale);
		glColor3f(1.0, 1.0, 1.0);
		glRasterPos2f(0 , 0 + font.LineHeight());
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
			char* text = (char*)str.c_str();
			font.Render(text);
		}

		glPopMatrix();
		
	}

	System::Void RawDataVisualization::DrawText_FTGL_withColor(int n,int x, int y, float scale, float r, float g, float b)
	{
		glPushMatrix();

		glTranslatef(x, y-30, 0);
		glScalef(1.0+scale_x[1],1.0+scale_y[1],1.0+scale_z[1]);	
	
		//float font_size = 10*(scale_factor[2]+0.4+scale_x[2]);	
		font.FaceSize(scale);
		glColor3f(r, g, b);
		glRasterPos2f(0 , 0 + font.LineHeight());
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
			char* text = (char*)str.c_str();
			font.Render(text);
		}

		glPopMatrix();
		
	}

	System::Void RawDataVisualization::DrawStationName(int n, int x, int y, float scale)
	{
		//wcscpy(station_name[0],L"Weekdend Variance");
		
		float font_size = 20.0*scale;
		font_new->FaceSize(font_size);
		glPushMatrix();

		glScalef(1.0, -10.0, 1.0);
		glTranslatef(x, y, 3);
		//glRotatef(270, 0.0, 0.0, -1.0);
		//glRotatef(180, 0.0, 1.0, 0.0);
		//glRotatef(test++, 1.0, 0.0, 0.0);
		wchar_t strText[] = L"快給我中文Please"; 
		glColor3f(1.0, 1.0, 1.0);
		font_new->Render(strText);
		//font_new->Render(station_name[n]);

		glPopMatrix();
	}

	System::Void RawDataVisualization::DrawTime_FTGL(int index,int x, int y)
	{
		int hour;
		int five_minute_index;
		for(int i=0;i<preprocessing_data.hour_range.size();i++)
		{
			if(index>=preprocessing_data.hour_range[i].x && index<= preprocessing_data.hour_range[i].y)
			{
				hour = preprocessing_data.hour_index[i];
				if(hour == preprocessing_data.begin_hour && preprocessing_data.num_of_begin_hour<12)
					five_minute_index = index - preprocessing_data.hour_range[i].x + (12-preprocessing_data.num_of_begin_hour);
				else
					five_minute_index = index - preprocessing_data.hour_range[i].x;
				
				break;
			}
		}
		stringstream ss;
		ss << hour;
		string str = ss.str();
		char *hour_text = (char*)str.c_str();		

		strcat(hour_text,five_minutes[five_minute_index]);

		glPushMatrix();

		float font_size = 10*(scale_factor[1]+0.3+scale_x[1]);
		font.FaceSize(font_size);
		glColor3f(1.0, 1.0, 1.0);
		glRasterPos2f(x , y-20.0 + font.LineHeight());
		font.Render(hour_text);

		glPopMatrix();		
	}

	vector3 RawDataVisualization::Unprojection(vector2 _2Dpos){
		float Depth;
		int viewport[4];
		//double ModelViewMatrix[16];				//Model_view matrix
		double ProjectionMatrix[16];			//Projection matrix

		glPushMatrix();

		glGetIntegerv(GL_VIEWPORT, viewport);
		//glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, ProjectionMatrix);

		glPopMatrix();

		glReadPixels((int)_2Dpos.x , viewport[3] - (int)_2Dpos.y , 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &Depth);

		double X = _2Dpos.x;
		double Y = _2Dpos.y;
		double wpos[3] = {0.0 , 0.0 , 0.0};

		gluUnProject(X , ((double)viewport[3] - Y) , (double)Depth , ModelViewMatrix2 , ProjectionMatrix , viewport, &wpos[0] , &wpos[1] , &wpos[2]);

		return vector3(wpos[0] , wpos[1] , wpos[2]);
	}

	System::Void RawDataVisualization::RawDataMouseDown( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ){
			last_X[1] = e->X;
			last_Y[1] = e->Y;

			if(preprocessing_data.view_select_right_index==1)
			{
				if (e->Button == System::Windows::Forms::MouseButtons::Right)
				{
					FindPatternByTable2(e->X,e->Y);
					/////////////////////////////////////
					temp_line->x1 = e->X; // Line
					temp_line->y1 = e->Y;
					temp_line->x2 = e->X; // Line
					temp_line->y2 = e->Y;
					line_draw = 1; // start drawing drag line
				}
			}

			else if(e->Button == System::Windows::Forms::MouseButtons::Middle)
			{
				//FindPatternByTable2(e->X,e->Y);
				FindStation(e->X,e->Y);
			}

			if (e->Button == System::Windows::Forms::MouseButtons::Middle && !drag)
			{
				rect_x = e->X;
				rect_y = e->Y;
				drag = 1;
			}


	}
	System::Void RawDataVisualization::RawDataMouseWheel( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ){
			
		
			if (e->Delta < 0){

				scale_x[1]+=scale_size[1];
				scale_y[1]+=scale_size[1];
				scale_z[1]+=scale_size[1];
			}
			else{
				scale_x[1]-=scale_size[1];
				scale_y[1]-=scale_size[1];
				scale_z[1]-=scale_size[1];		
			}
		
			
	}
	System::Void RawDataVisualization::RawDataMouseMove( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ){
			//FindStation(e->X,e->Y);


			
			if (e->Button == System::Windows::Forms::MouseButtons::Left)
			{		
					vector2 Move(e->X - last_X[1] , e->Y - last_Y[1]);
					//if (Move.length() < 500.0f)
					//{
						move_x[1] += 0.3*Move.x;
						move_y[1] += 0.3*Move.y;
						//move_x[1] = Move.x;
						//move_y[1] = Move.y;
					//}
					
			}	

			else if (e->Button == System::Windows::Forms::MouseButtons::Right)
			{
				if(preprocessing_data.view_select_right_index==1)
				{
					if(line_draw)
					{
						temp_line->x2 = e->X;
						temp_line->y2 = e->Y;		
					}
				}
			}
	}

	System::Void RawDataVisualization::RawDataMouseUp( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ){
			last_X[1] = e->X;
			last_Y[1] = e->Y;

			if (e->Button == System::Windows::Forms::MouseButtons::Right)
			{
				if(preprocessing_data.view_select_right_index==1)
				{
					temp_line->x2 = e->X;
					temp_line->y2 = e->Y;

					FindPointInRect();
				}
			}

	}

	System::Void RawDataVisualization::RawDataMouseDoubleClick( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ){
			if (e->Button == System::Windows::Forms::MouseButtons::Left)
			{			
				//FindHistogram(e->X,e->Y);
				//System::Windows::Forms::MessageBox::Show("ha");	
			}
	}

	System::Void RawDataVisualization::DrawTitle_FTGL(int t,int x, int y)
	{
		strcpy(title[0],"Weekday");
		strcpy(title[1],"Weekend");
		strcpy(title[2],"Weekday Variance");
		strcpy(title[3],"Weekend Variance");
		strcpy(title[4],"Small");
		strcpy(title[5],"Large");
		

		glPushMatrix();

		float font_size = 16*(scale_factor[1]+0.4+scale_x[1]);
		font.FaceSize(font_size);
		glColor3f(1.0, 1.0, 1.0);
		glRasterPos2f(x , y + font.LineHeight());
		font.Render(title[t]);

		glPopMatrix();			
	}


	System::Void RawDataVisualization::DrawTitle_FTGL_vertically(int t,int x, int y)
	{
		strcpy(title[0],"Weekday");
		strcpy(title[1],"Weekend");
		strcpy(title[2],"Weekday Variance");
		strcpy(title[3],"Weekend Variance");
		strcpy(title[4],"Small");
		strcpy(title[5],"Large");
		//wcscpy(wchar_test[0],L"Weekdend Variance");
		
		float font_size = 16*(scale_factor[1]+0.4+scale_x[1]);
		font_english->FaceSize(font_size);
		glPushMatrix();

		glTranslatef(x, y, 0);
		glRotatef(270, 0.0, 0.0, -1.0);
		glRotatef(180, 0.0, 1.0, 0.0);
		//wchar_t strText[] = L"快給我中文Please"; 
		glColor3f(1.0, 1.0, 1.0);
		font_english->Render(title[t]);

		glPopMatrix();
		/*
		glPushMatrix();

		glTranslatef(x, y, 0);
		glRotatef(test++, 0, 0, 1);
		//glTranslatef(0, 5, 0);
		glTranslatef(-x, -y, 0);
		///test++;

		float font_size = 16*(scale_factor[1]+0.4+scale_x[1]);
		font.FaceSize(font_size);
		glColor3f(1.0, 1.0, 1.0);
		glRasterPos2f(x , y + font.LineHeight());
		font.Render(title[t]);

		glPopMatrix();			
		*/
	}

	System::Void RawDataVisualization::time_string()
	{
		strcpy(five_minutes[0],":00");
		strcpy(five_minutes[1],":05");
		strcpy(five_minutes[2],":10");
		strcpy(five_minutes[3],":15");
		strcpy(five_minutes[4],":20");
		strcpy(five_minutes[5],":25");
		strcpy(five_minutes[6],":30");
		strcpy(five_minutes[7],":35");
		strcpy(five_minutes[8],":40");
		strcpy(five_minutes[9],":45");
		strcpy(five_minutes[10],":50");
		strcpy(five_minutes[11],":55");
	}

	System::Void RawDataVisualization::clear()
	{
		//histogram_index.clear();

		raw_data_position_table.clear();

		preprocessing_data.select_station.clear();

		preprocessing_data.start_flag = false;

		int dim_index[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88,29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,
							35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77,22,7,19,
							24,8,11,16,41,40,39,36,130,97,82,80,42,131,93,92,86,27,65,61,58,57,56,33,85,71,68,94,9};			
		int station_num = sizeof(dim_index)/sizeof(dim_index[0]);
		int home[] = {29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77};//43
		int home_num = sizeof(home)/sizeof(home[0]);
		int work_school[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88};//20
		int work_school_num = sizeof(work_school)/sizeof(work_school[0]);
		for(int i=0;i<station_num;i++) dim_label[ dim_index[i] ] = 1;
		for(int i=0;i<home_num;i++)  dim_label[ home[i] ] = 2;
		for(int i=0;i<work_school_num;i++)  dim_label[ work_school[i] ] = 3;
		//raw_data_position_table.resize(100);
		//for(int i=0;i<100;i++) raw_data_position_table[i].resize(31);

	}

	System::Void RawDataVisualization::clear_view()
	{
		histogram_index.clear();
	}
}