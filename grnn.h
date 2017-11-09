#ifndef GRNN_H
#define GRNN_H

#include "datapoint.h"

#define POPULATION 768
#define FEAT_CNT 95

class Std_Dev {
	int act_pop;
	double* fitness;
	double* sigma;
	int* rank;
	
	Std_Dev(int pop);
	Std_Dev(Data_Point trng_set, int pop);
	void set_fitness(double fit, int i);
	void assess_fitness(Data_Point trng_set[], int child);
	void select_parents(int act_pop);
	double procreate(Data_Point trng_set[], int k);
	
};

class GRNN {
	public:
	Data_Point pred_set[POPULATION];
	double sigma;
	
	GRNN();
	GRNN(Data_Point trng_set[POPULATION], double sig);
	double classify(Data_Point trng_set[POPULATION], Data_Point point);
};

#endif