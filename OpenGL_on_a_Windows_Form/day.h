#ifndef DAY_H
#define DAY_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "hour.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <highgui.h>
#include "cv.h"

using namespace std;
using namespace cv;

class day{
public:
	vector<hour> hour_vec;
	int this_year;
	int this_month;
	int this_date;

	bool IsHoliday;

	Mat river_table_current;
	Mat river_table_next;
};

#endif