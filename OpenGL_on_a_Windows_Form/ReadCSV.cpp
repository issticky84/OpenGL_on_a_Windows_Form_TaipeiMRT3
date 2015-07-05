#include "stdafx.h"
#include "ReadCSV.h"
#include <string.h>
#include <fstream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <highgui.h>
#include "cv.h"

using namespace cv;

void ReadCSV::clear()
{
	raw_data.clear();
	attribute_index.clear();
}

void ReadCSV::read_MRT_position(char* file_name)
{
	FILE* file = NULL;
	file = fopen("MRT_position.txt","r");
	if(!file)
	{
		cout << "can't open config file!";
		exit(1);
	}

	char line[1024];
	char *token;
	fgets(line,1024,file);

	MRT_position.resize(199);
	for(int i=0;i<MRT_position.size();i++) MRT_position[i].resize(2);

	int code, position_x, position_y;
	char name[100];
	while( !feof(file) )
	{
		fscanf(file, "%d %s %d %d\n",&code,name,&position_x,&position_y);

		//cout << code << " " << position_x << " " << position_y << endl;
		//circle(image, Point(position_x, position_y),4, Scalar(0,255,255),2, 8,0);
		MRT_position[code][0] = position_x;
		MRT_position[code][1] = position_y;
	}
}

void ReadCSV::test_image()
{
	Mat image;
    image = imread("MRT_Map.jpg", CV_LOAD_IMAGE_COLOR); 
	if(! image.data )                              // Check for invalid input
	{
		cout <<  "Could not open or find the image" << std::endl ;
		exit(1);
	}

	circle(image, Point(28,18),6, Scalar(0,255,255),3, 8,0);
	imwrite( "MRT_Map2.jpg", image );
}

void ReadCSV::read_file_all()
{
	hour_amount = 0;
	day_amount = 0;
	int month_day[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	month_vec.resize(12);//////////////////////////////
	for(int i=0;i<month_vec.size();i++)
	{
		month_vec[i].day_vec.resize(month_day[i]);
		hour_amount+=month_day[i];
		for(int j=0;j<month_vec[i].day_vec.size();j++)
		{
			month_vec[i].day_vec[j].hour_vec.resize(24);
			day_amount++;
		}
	}

	hour_amount*=24;

	read_single_file2("201101");
	read_single_file2("201102");
	read_single_file2("201103");
	read_single_file2("201104");
	read_single_file2("201105");
	read_single_file2("201106");
	read_single_file2("201107");	
	read_single_file2("201108");
	read_single_file2("201109");
	read_single_file2("201110");
	read_single_file2("201111");
	read_single_file2("201112");
}

void ReadCSV::read_single_file2(char* file_name)
{
	FILE* file = NULL;
	file = fopen(file_name,"r");
	//fopen_s(&file,file_name,"r");
	if(!file)
	{
		cout << "Can't open config file!" << endl;
		perror(file_name);
		exit(1);
	}
	
	
	char line[LENGTH];
	char *token;
	fgets(line,LENGTH,file); //ignore title

	int code,month,date,hour,enter,out;
	
	//fscanf(file, "%s,%s,%s,%s,%s\n", str[0],str[1],str[2],str[3],str[4]);
	while (!feof(file))
	{
		fscanf(file, "%d %d %d %d %d %d\n",&code,&month,&date,&hour,&enter,&out);
		//cout << code << " " << month << " " << date << " " << enter << " " << out << endl;
		month_vec[month-1].day_vec[date-1].hour_vec[hour].enter[code] = enter;
		month_vec[month-1].day_vec[date-1].hour_vec[hour].out[code] = out;
		month_vec[month-1].this_month = month;
	}

	fclose(file);
}

void ReadCSV::read_holiday_event(char* file_name)
{
	FILE* file;
	file = fopen(file_name,"r");
	if(!file)
	{
		cout << "Can't open config file!" << endl;
		perror(file_name);
		exit(1);
	}

	char line[LENGTH];
	char *token;
	while(!feof(file))
	{
		fgets(line,LENGTH,file);
		token = strtok(line," \/");
		raw_data.push_back(vector<float> (1));
		//printf("%s ",token);
		while(token!=NULL)
		{
			raw_data.back().push_back(atof(token));
			//printf("%s ",token);

			token = strtok(NULL," \/");
		}
	}

	fclose(file);

	for(int i=0;i<raw_data.size();i++)
	{
		holiday holiday_obj;
		for(int j=0;j<raw_data[i].size();j++)
		{
			if(j==1)
				holiday_obj.year = raw_data[i][1];
			else if(j==2)
				holiday_obj.month = raw_data[i][2];
			else if(j==3)
				holiday_obj.date = raw_data[i][3];
		}
		holiday_vec.push_back(holiday_obj);
	}
	/*
	for(int i=0;i<holiday_vec.size();i++)
	{
		cout << holiday_vec[i].year << " " << holiday_vec[i].month << " "  << holiday_vec[i].date << endl;
	}
	*/
	raw_data.clear();
}

void ReadCSV::read_file_list(char* file_name)
{
	FILE* file;
	file = fopen(file_name,"r");
	if(!file)
	{
		cout << "Can't open config file!" << endl;
		perror(file_name);
		exit(1);
	}

	char line[LENGTH];
	char *token;
	while(!feof(file))
	{
		fgets(line,LENGTH,file);
		token = strtok(line," \n");
		//printf("%s ",token);
		while(token!=NULL)
		{
			//printf("%s ",token);
			read_single_file(token);
			token = strtok(NULL," \n");
		}
	}

	fclose(file);
}

void ReadCSV::read_single_file(char* file_name)
{
	/*
	FILE* file = NULL;
	file = fopen(file_name,"r");
	//fopen_s(&file,file_name,"r");
	if(!file)
	{
		cout << "Can't open config file!" << endl;
		perror(file_name);
		exit(1);
	}
	
	char line[LENGTH];
	char *token;
	//fgets(line,LENGTH,file); //ignore sep=
	fgets(line,LENGTH,file); //ignore title

	vector<string> title_name;
	token = strtok(line," \t");
	while(token!=NULL)
	{
		title_name.push_back(token);
		token = strtok(NULL," \t");
	}
	for(int i=0;i<title_name.size();i++)
	{
		//cout << title_name[i] << " ";
		//if(title_name[i].compare("營運日")==0){ attribute_index.push_back(i); }
		//else if(title_name[i].compare("星期")==0){ attribute_index.push_back(i); }
		//else if(title_name[i].compare("紅線運量(人次)")==0){ attribute_index.push_back(i); }
		//else if(title_name[i].compare("橘線運量(人次)")==0){ attribute_index.push_back(i); }
		//else if(title_name[i].compare("總運量(人次)")==0){ attribute_index.push_back(i); }
		//cout << title_name[i] << " ";
	}

	//for(int i=0;i<attribute_index.size();i++)
	//	cout << attribute_index[i] << " ";
	//cout << endl;

	while(!feof(file))
	{
		fgets(line,LENGTH,file);
		token = strtok(line," \t\/");
		raw_data.push_back(vector<float> (1));
		//printf("%s ",token);
		while(token!=NULL)
		{
			raw_data.back().push_back(atof(token));
			token = strtok(NULL," \t\/");
		}
	}


	month month_obj;
	for(int i=0;i<raw_data.size();i++)
	{	
		if(i==0)
		{
			month_obj.this_year = raw_data[i][1];
		}
		
		day day_obj;
		day_obj.data_dim = 0;
		int dim = 0;
		for(int j=0;j<raw_data[i].size();j++)
		{	
			day_obj.IsHoliday = false;

			if(j==1)  day_obj.year = raw_data[i][j];
			else if(j==2)
			{
				month_obj.this_month = raw_data[i][j];
				day_obj.month = raw_data[i][j];
			}
			else if(j==3)  day_obj.date = raw_data[i][j];
			//else if(j==10) //airport總運量
			//else if(j>=4) //沒有"星期"欄位
			//else if(j>=5) //有"星期"欄位
			//else if(j==7) //MRT總運量
			else if(j==5 || j==6) //MRT紅線&橘線
			//else if(j==7 || j==8) //MRT總運量&高雄雨量
			{
				day_obj.data[dim++] = raw_data[i][j];
				day_obj.data_dim++;
			}
			//else if(j==5)  day_obj.red_traffic_volulme = raw_data[i][j];
			//else if(j==6)  day_obj.orange_traffic_volulme = raw_data[i][j];
			//else if(j==7)  day_obj.total_traffic_volume = raw_data[i][j];
			
		}
		month_obj.day_vec.push_back(day_obj);
	}

	month_vec.push_back(month_obj);

	raw_data.clear();

	fclose(file);
	*/
}

void ReadCSV::read_raw_data(char file_name[])
{
	vector<string> title_name;

	FILE *csv_file;
	csv_file = fopen(file_name,"r");
	if(!csv_file) 
	{
		cout << "Can't open config file!" << endl;
		system("pause");
		exit(1);
	}

	char line[LENGTH];
	char *token;
	int i,j;
	i = j = 0;
	fgets(line,LENGTH,csv_file); //ignore sep=
	fgets(line,LENGTH,csv_file); //ignore title
	
	token = strtok(line,";");
	while(token!=NULL)
	{
		title_name.push_back(token);
		token = strtok(NULL,";");
	}
	for(int i=0;i<title_name.size();i++)
	{
		//cout << title_name[i] << " ";
		if(title_name[i].compare("GRAVITY X (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("GRAVITY Y (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("GRAVITY Z (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("LINEAR ACCELERATION X (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("LINEAR ACCELERATION Y (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("LINEAR ACCELERATION Z (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("GYROSCOPE X (rad/s)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("GYROSCOPE Y (rad/s)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("GYROSCOPE Z (rad/s)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("LOCATION Latitude : ")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("LOCATION Longitude : ")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("YYYY-MO-DD HH-MI-SS_SSS\n")==0){ time_index = i; }
	}

	//for(int i=0;i<attribute_index.size();i++)
	//	cout << attribute_index[i] << endl;

	cout << "time_index " << time_index << endl;

	while(!feof(csv_file))
	{
		fgets(line,LENGTH,csv_file);
		token = strtok(line,";");
		raw_data.push_back(vector<float> (1));
		//printf("%s ",token);
		while(token!=NULL)
		{
			raw_data.back().push_back(atof(token));
			//token = strtok(NULL," ;:");
			token = strtok(NULL," ;:");
		}
	}

	//for(int i=0;i<33;i++)
	//{
	//	cout << raw_data[0][i] << " ";
	//}
	//cout << endl;
	//cout << raw_data[0][1] << " " << raw_data[0][29] << " " << raw_data[0][30] << " " << raw_data[0][31] << " " << raw_data[0][32]  << " " << raw_data[0][33] << endl;
	//cout << raw_data[0][1] << " " << raw_data[0][27] << " " << raw_data[0][28] << " " << raw_data[0][29] << " " << raw_data[0][30]  << " " << raw_data[0][31] << endl;
	//cout << raw_data[0][1] << " " << raw_data[0][31] << " " << raw_data[0][32] << " " << raw_data[0][33] << " " << raw_data[0][34]  << " " << raw_data[0][35] << endl;
	//29:Year,30:Hour,31:Minute,32:second,33:millisecond

	cout << "Csv Data Size: " << raw_data.size() <<endl;
	//cout << raw_data[0].size() << endl;

	raw_data_size = raw_data.size();

	fclose(csv_file);
}