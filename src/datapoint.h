#ifndef DATA_H
#define DATA_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdlib.h>
#include <math.h>

#define FEAT_CNT 95
#define POPULATION 200
#define TRAINING_SET 595

using namespace std;

class Data_Point {
    public:
    int pnt_id; // An identification number
	  double clsfr; // Classifier
    float feat_vecs[FEAT_CNT]; // Array for each of the 95 features of each element

	Data_Point();
	//Data_Point(string line);
	Data_Point(string line, int i);
	float distance(Data_Point b, double weights[]);
	int init_trng_set(Data_Point trng_set[]);
	void init_test_set(Data_Point test_set[], int j);
};

#endif
