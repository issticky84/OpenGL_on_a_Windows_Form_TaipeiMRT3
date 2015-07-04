#ifndef DAY_H
#define DAY_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "hour.h"

using namespace std;

class day{
public:
	vector<hour> hour_vec;
	int this_year;
	int this_month;
	int this_date;

	bool IsHoliday;
};

#endif