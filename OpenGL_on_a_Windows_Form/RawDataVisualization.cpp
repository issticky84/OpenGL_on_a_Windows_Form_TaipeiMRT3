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
								24,8,11,16,41,40,39,36,130,97,82,80,42,131,93,92,86,27,65,61,58,57,56,33,85,71,68};			
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
			glTranslatef(40.0+move_x[1],100.0+move_y[1],0.0+move_z[1]);
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

					DrawText_FTGL(this_month+1,-30,y_position_text,16.0);
					DrawText_FTGL(this_day+1,-15,y_position_text,16.0);
					y_position_text+=10000;

					
					for(int i=5;i<23;i++)
					{
						int color_index = 0;
						//DrawText_FTGL(i,i*15-5,y_position+750,12.0);
						int tag = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].cluster_tag;
						DrawText_FTGL_withColor(i,i*15-5,y_position+750,12.0, preprocessing_data.rgb_mat3.at<float>(tag,0),
																			  preprocessing_data.rgb_mat3.at<float>(tag,1),
																			  preprocessing_data.rgb_mat3.at<float>(tag,2));

						for(int j=preprocessing_data.select_station.size()-1;j>=0;j--)
						{
							//int hour_data_current = preprocessing_data.month_vec[this_month].day_vec[this_day].river_table_current.at<float>(i,j);
							//int hour_data_next = preprocessing_data.month_vec[this_month].day_vec[this_day].river_table_next.at<float>(i,j);
							int hour_data_current = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].enter[ preprocessing_data.select_station[j] ];
							int hour_data_next = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].enter[ preprocessing_data.select_station[j] ];

							glLineWidth(3.0); 

							glPushMatrix(); 
							glBegin(GL_LINES); 
							glColor3f(preprocessing_data.data_color[color_index][0],preprocessing_data.data_color[color_index][1],preprocessing_data.data_color[color_index][2]);  
								glVertex3f((i+1)*15,y_position-hour_data_next,0); 
								glVertex3f(i*15,y_position-hour_data_current,0);						
							glEnd();
							glPopMatrix();
							/*
							glPushMatrix(); 
							glBegin(GL_QUADS); 
							glColor3f(preprocessing_data.data_color[color_index][0],preprocessing_data.data_color[color_index][1],preprocessing_data.data_color[color_index][2]);  
								glVertex3f(i*15,y_position,0);
								glVertex3f((i+1)*15,y_position,0);
								glVertex3f((i+1)*15,y_position-hour_data_next,0); 
								glVertex3f(i*15,y_position-hour_data_current,0);						
							glEnd();
							glPopMatrix();
							*/
							color_index++;
						}

						//out
						color_index = 0;
						//DrawText_FTGL(i,300+i*15-5,y_position+750,12.0);
						DrawText_FTGL_withColor(i,300+i*15-5,y_position+750,12.0, preprocessing_data.rgb_mat3.at<float>(tag,0),
																				  preprocessing_data.rgb_mat3.at<float>(tag,1),
																			      preprocessing_data.rgb_mat3.at<float>(tag,2));
						for(int j=preprocessing_data.select_station.size()-1;j>=0;j--)
						{
							//int hour_data_current = preprocessing_data.month_vec[this_month].day_vec[this_day].river_table_current.at<float>(i,j);
							//int hour_data_next = preprocessing_data.month_vec[this_month].day_vec[this_day].river_table_next.at<float>(i,j);
							int hour_data_current = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].out[ preprocessing_data.select_station[j] ];
							int hour_data_next = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].out[ preprocessing_data.select_station[j] ];

							glLineWidth(3.0); 

							glPushMatrix(); 
							glBegin(GL_LINES); 
							glColor3f(preprocessing_data.data_color[color_index][0],preprocessing_data.data_color[color_index][1],preprocessing_data.data_color[color_index][2]);  
								glVertex3f(300+(i+1)*15,y_position-hour_data_next,0); 
								glVertex3f(300+i*15,y_position-hour_data_current,0);						
							glEnd();
							glPopMatrix();

							color_index++;
						}
					}
					/*
					for(int i=0;i<23;i++)
					{
						if(preprocessing_data.data_dim>=6)
						{
							int hour_data_current = preprocessing_data.month_vec[this_month].day_vec[this_day].river_table_current.at<float>(i,5);
							int hour_data_next = preprocessing_data.month_vec[this_month].day_vec[this_day].river_table_next.at<float>(i,5);
							//int hour_data_current = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].data[0]
							//					  + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].data[1]
							//					  + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].data[2]
							//					  + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].data[3]
							//					  + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].data[4]
							//					  + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].data[5];
							//int hour_data_next = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].data[0]
							//				   + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].data[1]
							//				   + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].data[2]
							//				   + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].data[3]
							//				   + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].data[4]
							//				   + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].data[5];
							glPushMatrix(); 
							glBegin(GL_QUADS); 
							glColor3f(preprocessing_data.data_color[5][0],preprocessing_data.data_color[5][1],preprocessing_data.data_color[5][2]);  
								glVertex3f(i*15,y_position,0);
								glVertex3f((i+1)*15,y_position,0);
								glVertex3f((i+1)*15,y_position-hour_data_next,0); 
								glVertex3f(i*15,y_position-hour_data_current,0);						
							glEnd();
							glPopMatrix();
						}
					}
					*/
					int tag = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[23].cluster_tag;
					DrawText_FTGL_withColor(23,23*15-5,y_position+750,12.0, preprocessing_data.rgb_mat3.at<float>(tag,0),
																			preprocessing_data.rgb_mat3.at<float>(tag,1),
																			preprocessing_data.rgb_mat3.at<float>(tag,2));
					DrawText_FTGL_withColor(23,300+23*15-5,y_position+750,12.0, preprocessing_data.rgb_mat3.at<float>(tag,0),
																				preprocessing_data.rgb_mat3.at<float>(tag,1),
																				preprocessing_data.rgb_mat3.at<float>(tag,2));
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

			for(int day=0; day<preprocessing_data.dim ;day++)
			{
				int dim_index[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88,29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,
								   35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77,22,7,19,
								   24,8,11,16,41,40,39,36,130,97,82,80,42,131,93,92,86,27,65,61,58,57,56,33,85,71,68};					
				//int home[] = {29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77};//43
				//int home_num = sizeof(home)/sizeof(home[0]);
				//int work_school[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88};//20
				//int work_school_num = sizeof(work_school)/sizeof(work_school[0]);
				//for(int i=0;i<preprocessing_data.dim;i++) dim_label[ dim_index[i] ] = 1;
				//for(int i=0;i<home_num;i++)  dim_label[ home[i] ] = 2;
				//for(int i=0;i<work_school_num;i++)  dim_label[ work_school[i] ] = 3;

				int dim_num = dim_index[day];
				glPointSize( 8.0 );

				glPushMatrix();
				glBegin( GL_POINTS );
				if(dim_label[dim_num] == 2)
					glColor3f( 1.0f, 0.0, 0.0f );
				else if(dim_label[dim_num] == 3)
					glColor3f( 0.0f, 1.0, 0.0f );
				else if(dim_label[dim_num] == 4)
					glColor3f( 0.0f, 0.0, 1.0f );
				else
					glColor3f( 1.0f, 1.0, 1.0f );
				glVertex2f(80 + 1.5*preprocessing_data.position_on_2D.at<double>(day,0), 70 + 1.5*preprocessing_data.position_on_2D.at<double>(day,1) );
				glEnd();
				glPopMatrix();
					
				//DrawCircle(preprocessing_data.position_on_2D.at<double>(day,0),preprocessing_data.position_on_2D.at<double>(day,1), 2.0, 1, 0, 0);
				//DrawCircle(preprocessing_data.position_on_2D.at<double>(day,0),preprocessing_data.position_on_2D.at<double>(day,1), 2.0, (rand() % 11)/10.0, (rand() % 11)/10.0, (rand() % 11)/10.0);
			}	
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
		   glLineWidth (1.5);
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
				themeriverView();
			}
			else if(preprocessing_data.view_select_right_index==1)
			{
				stationView();

				//if( line_draw )
				//{

					glPushMatrix();

					glBegin(GL_LINES);
					glVertex2d(temp_line->x1,temp_line->y1 ); 
					glVertex2d(temp_line->x2,temp_line->y1 ); 
					glEnd();

					glBegin(GL_LINES);
					glVertex2d(temp_line->x1,temp_line->y1 ); 
					glVertex2d(temp_line->x1,temp_line->y2 ); 
					glEnd();

					glBegin(GL_LINES);
					glVertex2d(temp_line->x1,temp_line->y2 ); 
					glVertex2d(temp_line->x2,temp_line->y2 ); 
					glEnd();

					glBegin(GL_LINES);
					glVertex2d(temp_line->x2,temp_line->y1 ); 
					glVertex2d(temp_line->x2,temp_line->y2 ); 
					glEnd();

					glPopMatrix();
				//}
			}

			SwapOpenGLBuffers();
			
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

		for(int day=0; day<preprocessing_data.dim ;day++)
		{
			int x = 80 + 1.5*preprocessing_data.position_on_2D.at<double>(day,0);
			int y = 70 + 1.5*preprocessing_data.position_on_2D.at<double>(day,1);
			if(x >= pos1.x && x <= pos2.x && y >= pos1.y && y <= pos2.y)
			{
				int dim_index[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88,29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,
								   35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77,22,7,19,
								   24,8,11,16,41,40,39,36,130,97,82,80,42,131,93,92,86,27,65,61,58,57,56,33,85,71,68};

				//System::Windows::Forms::MessageBox::Show( dim_index[day] + " ");

				dim_label[ dim_index[day] ] = 4;

				if ( !preprocessing_data.check_duplicated_station( dim_index[day]) )
					preprocessing_data.select_station.push_back( dim_index[day] );				
			}
		}
	}

	System::Void RawDataVisualization::FindPatternByTable2(int x,int y){
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
			int x = preprocessing_data.position_on_2D.at<double>(day,0);
			int y = preprocessing_data.position_on_2D.at<double>(day,1);
			x *= (scale_factor[1] + scale_x[1]);
			y *= (scale_factor[1] + scale_y[1]);
			x += move_x[1];
			y += move_y[1];

			//if( abs(pos_3D.x-x) <= 1.0 && abs(pos_3D.y-y) <= 1.0 )
			if( (pos_3D.x-x) <= 2.3 && (pos_3D.x-x) >=0 &&  (pos_3D.y-y) <= 2.3 && (pos_3D.y-y) >=0 )
			{
				int dim_index[] = {31,98,30,23,90,10,12,129,55,54,53,52,51,50,42,132,91,89,133,88,29,28,26,25,21,13,14,15,17,18,70,69,66,64,63,62,60,59,43,38,37,
								   35,34,32,174,175,176,177,178,128,45,46,47,48,96,95,85,84,83,81,79,78,77,22,7,19,
								   24,8,11,16,41,40,39,36,130,97,82,80,42,131,93,92,86,27,65,61,58,57,56,33,85,71,68};

				//System::Windows::Forms::MessageBox::Show( dim_index[day] + " ");

				dim_label[ dim_index[day] ] = 4;
				//if(dim_label[ dim_index[day] ] != 4) dim_label[ dim_index[day] ] = 4;
				//else if(dim_label[ dim_index[day] ] == 4) dim_label[ dim_index[day] ] = 1;

				if ( !preprocessing_data.check_duplicated_station( dim_index[day]) )
					preprocessing_data.select_station.push_back( dim_index[day] );
			}
		}
		/*
		for(int i=0;i<raw_data_position_table.size();i++)
		{
			//if(histogram_index[i] > preprocessing_data.month_vec.size() || histogram_index[i] < 0) break; 
			//if(histogram_index[i] > read_csv.raw_data_size-1) break; ////////bug//////(index out of scrope)
			for(int j=0;j<raw_data_position_table[i].size();j++)
			{
				//if(histogram_index[i] > preprocessing_data.month_vec.size() || histogram_index[i] < 0) break; 
				//if(histogram_index[i]+j > read_csv.raw_data_size-1) break;
				if(pos_3D.x >= raw_data_position_table[i][j].x && pos_3D.x <= raw_data_position_table[i][j].z && pos_3D.y >= raw_data_position_table[i][j].y && pos_3D.y <= raw_data_position_table[i][j].w)
				{
					System::Windows::Forms::MessageBox::Show( (histogram_index[i]).ToString() + " " + (j+1).ToString());			
					//System::Windows::Forms::MessageBox::Show(histogram_index[i] + " " + j + " " + 
					//read_csv.raw_data[ histogram_index[i]+j][1].ToString() + " " + read_csv.raw_data[ histogram_index[i]+j][2].ToString() + " " + read_csv.raw_data[ histogram_index[i]+j][3].ToString());
					//raw_data_index.push_back(histogram_index[i]+j);
					vector<int> temp;
					temp.push_back(histogram_index[i]);
					temp.push_back(j);
					raw_data_index_2D.push_back(temp);
				}
			}
		}*/
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

	System::Void RawDataVisualization::DrawCircle(int x, int y, float radius, float r, float g, float b)
	{
		int i;
		int triangleAmount = 30; //# of triangles used to draw circle
	
		//GLfloat radius = 0.8f; //radius
		GLfloat twicePi = 2.0f * 3.14;
	
		glBegin(GL_TRIANGLE_FAN);
			glColor3f(r,g,b);
			glVertex2f(x, y); // center of circle
			for(i = 0; i <= triangleAmount;i++)
			{ 
				glVertex2f(
					x + (radius * cos(i *  twicePi / triangleAmount)), 
					y + (radius * sin(i * twicePi / triangleAmount))
				);
			}
		glEnd();		
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
			if (e->Button == System::Windows::Forms::MouseButtons::Left)
			{		
					vector2 Move(e->X - last_X[1] , e->Y - last_Y[1]);
					if (Move.length() < 500.0f)
					{
						move_x[1] = Move.x;
						move_y[1] = Move.y;
					}
					
			}	

			else if (e->Button == System::Windows::Forms::MouseButtons::Right)
			{
				if(line_draw)
				{
					temp_line->x2 = e->X;
					temp_line->y2 = e->Y;		
				}				
			}
	}

	System::Void RawDataVisualization::RawDataMouseUp( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ){
			last_X[1] = e->X;
			last_Y[1] = e->Y;

			if (e->Button == System::Windows::Forms::MouseButtons::Right)
			{
				temp_line->x2 = e->X;
				temp_line->y2 = e->Y;
				//line_draw = 0; // stop drawing drag line

				FindPointInRect();
			}

			//if (e->Button == System::Windows::Forms::MouseButtons::Middle && drag)
			//{
			//	drag = 0;
			//	ROI->h = e->X - rect_x;
			//	ROI->w = e->X - rect_y;
			//	ROI->x = rect_x;
			//	ROI->y = rect_y;
			//	System::Windows::Forms::MessageBox::Show(ROI->x + " " + ROI->y + " " + e->X + " " +  e->Y );
			//	draw_roi = true;
			//}
	}

	System::Void RawDataVisualization::RawDataMouseDoubleClick( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ){
			if (e->Button == System::Windows::Forms::MouseButtons::Left)
			{			
				//FindHistogram(e->X,e->Y);
				//System::Windows::Forms::MessageBox::Show("ha");	
			}
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
							24,8,11,16,41,40,39,36,130,97,82,80,42,131,93,92,86,27,65,61,58,57,56,33,85,71,68};			
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