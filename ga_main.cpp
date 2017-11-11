#include "./src/featga.h"

int main() {
    int pop_size = 20;
    int num_parents = 2;
    int num_data_points = 200;
    double classifications[num_data_points];
    
    string filepath;
    
    srand(time(0));
    
    for (int i = 0; i < 10; i++) {
        filepath = "bin/ga_output/best200_" + to_string(i) + ".txt";
        ofstream output(filepath.c_str());
        
        FeatGA ga;
        cout << "Starting Genetic Algorithm" << endl;
        cout << "Saving classifications of Initial Population" << endl;
        printf("Best Fitness So Far: %f\n", ga.best_fitness);
        for (int j = 0; j < pop_size; j++) {
            classifications[j] = ga.population[j].clsfr;
            cout << "Initial population member " << j << " classification: " << classifications[j] << endl;
            //output << classifications[j] << "\n";
        }
    
        cout << "Saving classifications of children" << endl;
        for (int j = 0; j < num_data_points; j++) {
            ga.select_parents();
            classifications[j] =  ga.procreate();
            printf("Best Member Fitness: %f\n", ga.best_fitness);
            output << ga.best_fitness << "\n";
        }
        output.close();
    }
}
