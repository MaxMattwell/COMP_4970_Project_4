#include "datapoint.h"
#include "grnn.h"
#include "featga.h"



FeatGA::FeatGA() {
    pop_size = POP_SIZE;
    num_parents = NUM_PARENTS;
    best_fitness = 1;
    
    Data_Point np = Data_Point();
    np.init_trng_set(trng_set);
    
    for (int i = 0; i < pop_size; i++) {
        cout << "Generating member " << i << endl;
        Data_Point new_member(trng_set);
        cout << "Saving member " << i << endl;
        population[i] = new_member;
        for (int j = 0; j < FEAT_CNT; j++) {
            //cout << "Member " << i << " element " << j << ": " << population[i].feat_vecs[j] << endl;
        }
        cout << "Member " << i << " classification: " << population[i].clsfr << endl;
        if (abs(population[i].clsfr) < best_fitness) {
            best_fitness = abs(population[i].clsfr);
        }
    }

}

void FeatGA::select_parents() {
    int tourn_number = 5;
    
    //srand(time(0));
    
    for (int i = 0; i < num_parents; i++) {
        int best_fitness = 1;
        Data_Point candidate;
        parents[i] = candidate;
        for (int j = 0; j < tourn_number; j++) {
            candidate = population[rand() % pop_size];
            if (abs(candidate.clsfr) < best_fitness) {
                best_fitness = abs(candidate.clsfr);
                parents[i] = candidate;
            }
        }
        //cout << "Fitness of Parent " << i << ": " << parents[i].clsfr << endl;
    }
}

double FeatGA::procreate() {
    Data_Point child;
    double parent1_element;
    double parent2_element;
    
    for (int i = 0; i < FEAT_CNT; i++) {
        parent1_element = parents[1].feat_vecs[i];
        parent2_element = parents[2].feat_vecs[i];
        //cout << "Parent 1 feature " << i << ": " << parent1_element << endl;
        //cout << "Parent 2 feature " << i << ": " << parent2_element << endl;
        if (parent1_element > parent2_element) {
            child.feat_vecs[i] = ((double) rand()) * (parent1_element - parent2_element) / (double) RAND_MAX + parent2_element;
        }
        else {
            child.feat_vecs[i] = ((double) rand() / RAND_MAX) * (parent2_element - parent1_element) + parent1_element;
        }
        //cout << "Child feature " << i << ": " << child.feat_vecs[i] << endl;
    }
    
    GRNN trng_ntwk = GRNN(trng_set, 0.1237);
    child.clsfr = trng_ntwk.classify(trng_set, child);
    
    int worst_index;
    double worst_fitness = 0;
    
    for (int i = 0; i < pop_size; i++) {
        if (abs(population[i].clsfr) > worst_fitness) {
            worst_fitness = abs(population[i].clsfr);
            worst_index = i;
        }
        //cout << "Worst Fitness: " << worst_fitness << endl;
        //cout << "Worst Index: " << worst_index << endl;
    }
    
    //cout << "Fitness of Child: " << child.clsfr << endl;
    
    population[worst_index] = child;
    
    if (abs(child.clsfr) < best_fitness) {
        best_fitness = abs(child.clsfr);
    }
    
    //for (int j = 0; j < pop_size; j++) {
        //printf("Population member %d classification: %f\n", j, population[i].clsfr);
    //}
    
    return child.clsfr;
}

