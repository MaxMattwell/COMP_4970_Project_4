#include <math.h>
#include "grnn.h"
#include "datapoint.h"

/* General Regression Neural Network constructor
Receives a training set and a sigma value. Sigma is an empiraclly evaluated
constant used to make the prediction more accurate. Each of the elements in
the training set are classified and stored into an array of predictions for each
element.

inputs: training set, sigma value
outputs: N/A
updates: -MAC 11/5/17: refactoring
*/
GRNN::GRNN(Data_Point trng_set[POPULATION], double sig) {
	sigma = sig;
	
	//classify each of the training set and save to the predictions set
	int i, j;
	for(i = 0; i < POPULATION; i++) {
		pred_set[i].pnt_id = trng_set[i].pnt_id;
		
		for(j = 0; j < FEAT_CNT; j++) 
			pred_set[i].feat_vecs[j] = trng_set[i].feat_vecs[j];
		
		pred_set[i].clsfr = classify(trng_set, trng_set[i]);
	}
}

/*
updates: -MAC 11/5/17: refactoring
*/
double GRNN::classify(Data_Point trng_set[POPULATION], Data_Point point) {
	float gsan = 0, wgtd_gsan = 0;
	int i;
	
	for(i = 0; i < POPULATION; i++) {
		if(point.distance(trng_set[i]) != 0) {
			gsan += exp((-1 * pow(point.distance(trng_set[i]), 2)) / (2 * pow(sigma, 2)));
			wgtd_gsan += exp((-1 * pow(point.distance(trng_set[i]), 2)) / 
				(2 * pow(sigma, 2))) * trng_set[i].clsfr;
		}
	}
	
	return wgtd_gsan / gsan;
}
