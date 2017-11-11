#ifndef FEAT_GA
#define FEAT_GA

#include "datapoint.h"
#include "grnn.h"

#define POP_SIZE 20
#define NUM_PARENTS 2

class FeatGA {
public:
    int pop_size;
    int num_parents;
    Data_Point population[POP_SIZE];
    Data_Point parents[NUM_PARENTS];
    Data_Point trng_set[595];
    double best_fitness;
    
    FeatGA();
    void select_parents();
    double procreate();
};

#endif
