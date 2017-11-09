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
#define POPULATION 768 // Should probably be POP_MAX

using namespace std;

class Data_Point {
    public:
    int pnt_id; // An identification number
	double clsfr; // Classifier
    float feat_vecs[FEAT_CNT]; // Array for each of the 95 features of each element
    
	Data_Point();
	Data_Point(string line);
	Data_Point(string line, int i);
	float distance(Data_Point b);
	int init_trng_set(Data_Point trng_set[]);
	Data_Point init_test_set(Data_Point test_set[], int j, int y);
};

#endif