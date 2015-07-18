#ifndef Preprocessing_DataH
#define Preprocessing_DataH

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <highgui.h>
#include <vector>
#include "cv.h"
#include "mtxlib.h"//vector2

#include "month.h"
#include "holiday.h"
#include "group.h"

#define LENGTH 1000

using namespace std; 
using namespace cv;

class Preprocessing_Data
{
private:
	char file_csv_data[200];
	map<int,int> hour_map;
	//vector < vector<float> > raw_data;
	vector < vector<float> > lab_vertices;

	void output_mat_as_csv_file(char file_name[],Mat);
	void calcCovMat(Mat&, Mat&, Mat&);
	void reduceDimPCA(Mat&, int, Mat&, Mat&);
	//void read_raw_data();
	float degtorad(float);
	float norm_value(float,float,float);
	float DistanceOfLontitudeAndLatitude(float,float,float,float);
	void set_hour_data(vector < vector<float> > ,int time_title[]);
	Mat Gaussian_filter(vector < vector<float> >,int attribute_title[],int);
	void voting(int,Mat,int);
	Mat Position_by_MDS(Mat,int ,float);
	Mat lab_alignment(Mat,int);
	Mat lab_alignment_dim1(Mat,int);
	Mat lab_alignment_dim2(Mat,int);
	Mat lab_alignment_new(Mat,int);
	Mat lab_alignment_new_fast(Mat,int);
	Mat lab_alignment_new_dim1(Mat);
	Mat lab_alignment_new_dim2(Mat);

	void read_lab_csv();
	bool lab_boundary_test(float,float,float);
	Mat LAB2RGB(Mat);
	Mat compute_centroid(Mat);
	void gray2rgb(float,float& ,float& ,float&);
	Mat normalize_column(Mat);
	void output_mat_as_csv_file_float(char file_name[],Mat);
	void output_mat_as_csv_file_int(char file_name[],Mat);
	void output_mat_as_csv_file_double(char file_name[],Mat);
	void sort_by_color(int, Mat&, Mat&, Mat&);
	void interpolate_distance(Mat&,int);
	Mat MDS(Mat,int); 
	void distance_by_GMM(Mat&,Mat&,Mat,int);


	double TSP_boost_by_EdgeWeight(Mat, Mat&);
	double TSP_boost_by_EdgeWeight(Mat, Mat&, int, int);
	double TSP_boost_path_by_EdgeWeight(Mat, Mat&);
	double compute_dist(Mat,Mat,int);
	void sort_pattern_by_color_by_TSP_coarse_to_fine(Mat, Mat&);
	void sort_pattern_by_color_by_TSP_coarse_to_fine2(Mat, Mat&);
	void rearrange_mat_by_sort_color_index(Mat, Mat&, Mat&, Mat&);
	void sort_histogram_by_Ev_by_TSP_coarse_to_fine(Mat, Mat&);
	void sort_histogram_by_Ev_by_TSP_coarse_to_fine2(Mat, Mat&);
	void Position_by_histogram_sort_index(Mat&, Mat);
	void Position_by_histogram(Mat&, Mat);
	void voting_for_data(int,int,Mat);
	int count_digit(float);

	double TSP_boost_by_EdgeWeight_with_original(Mat, Mat&, int, int, vector<int>);
	double TSP_boost_by_EdgeWeight_with_original(Mat, Mat&, vector<int>);
	void TSP_boost_for_histogram_coarse_to_fine_multi(Mat, Mat, int);
	void TSP_for_index(Mat&, Mat& ,Mat&);
	Mat Ev_global;
	void TSP_group_connect(vector<group>,Mat&);
	vector<group> tree_group;
	Mat read_station_cov(char*);
	bool duplicate_position(int, int);

public:
	Preprocessing_Data();
	void start2(vector<month>,vector<holiday>,int);
	void start3(int,int,int);
	void Initial_selection_flag(bool, bool, bool, bool, bool, bool);

	int num_of_five_minutes;
	//int raw_data_size;
	vector <int> hour_data;
	Mat histogram;//int
	Mat histogram_draw;//int

	Mat rgb_mat;//float
	Mat rgb_mat3;//float
	Mat position;//double
	Mat raw_data_3D;//float
	Mat raw_data_mat;//float
	vector<vector2> hour_range;
	vector<int> hour_index;
	int num_of_begin_hour,begin_hour;
	bool waiting_flag;
	//bool select_gravity,select_linear_acc,select_gyro,select_distance;
	bool select_residential_in, select_residential_out, select_work_school_in, select_work_school_out, select_tourism_in, select_tourism_out;
	int progress_value;
	Mat lab;
	
	vector<month> month_vec;
	int day_amount;
	int hour_amount;
	Mat raw_data_3D_array[400];
	int zellers_congruence_for_week(int,int,int);
	bool check_holiday(vector<holiday>,int,int,int);

	int data_dim;
	bool data_dim_flag[6];
	Mat* find_month_and_day;
	vector< vector<float> > data_color;
	int comboBox_indx;
	int day_select_right_indx;
	void start_on_2D(int,int);
	Mat histogram_on_2D;
	Mat rgb_mat2_on_2D;
	Mat position_on_2D;
	int dim;
	void voting_for_data_on_2D(int,int,Mat);

	int view_select_left_index, view_select_right_index;

	vector<int> select_station;
	//Mat river_table_current;
	//Mat river_table_next;
	vector< vector<float> > MRT_position;
	void circle_MRT_station();

	bool start_flag;
	bool check_duplicated_station(int);

	int color_num;

	Mat station_rgb;
	Mat station_color_mat;
	//Mat station_color_mat_weekday;
	//Mat station_color_mat_weekend;

	int weekday_amount;
	int weekend_amount;

	float enter_total_avg;
	float out_total_avg;

	Mat cluster_center_raw;

	vector<int> Find_MRT_station(int x, int y);
	int sample_unit, sample_num_x, sample_num_y;
	Mat sample_color;
	
	Mat cov_color_bar;
	Mat cov_color_bar_weekday, cov_color_bar_weekend;

	vector<vector2>draw_circle_position;

};



#endif	