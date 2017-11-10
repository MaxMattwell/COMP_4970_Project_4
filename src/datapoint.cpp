#include "datapoint.h"
#include "grnn.h"

#define FEAT_CNT 95

 /* Constructor takes in a line from the dataset, saves the id, classifier,
 and saves each of the features into the vector. */
Data_Point::Data_Point() {  // Instantiation
	pnt_id = -1;
}
/*
Data_Point::Data_Point(string line) { // Initialization
	istringstream iss;
	iss.str(line);

	iss >> pnt_id;
	iss >> clsfr;

	int i;
	for(i = 0; i < FEAT_CNT; i++)
		iss >> feat_vecs[i];
}*/

Data_Point::Data_Point(string line, int i) {
	istringstream iss;
	iss.str(line);
	if (i == 0) {
		iss >> pnt_id;
		iss >> clsfr;

		int i;
		for(i = 0; i < FEAT_CNT; i++)
			iss >> feat_vecs[i];

	}
	else {
	iss >> pnt_id;
	for(i = 0; i < FEAT_CNT; i++)
		iss >> feat_vecs[i];

	}
}

/* Distance
MAC 10/15/17
Returns the distance between two data points. The distance is found by the
general Euclidian distance formula for FEAT_CNT dimensions.

input: data points b and local reference
output: Euclidian distance between two points
updates:
*/
float Data_Point::distance(Data_Point b, double weights[]) {
        float dist = 0;
        int i;
        for(i = 0; i < FEAT_CNT; i++)
            dist += weights[i] * pow((this->feat_vecs[i] - b.feat_vecs[i]), 2);

        return sqrt(dist);
}

/* Initialize Training Set
MAC 10/15/17
Reads in a file containing the whole dataset of the training data. For each of
the training elements stores the id, classifier, and all 95 feature values.

input: Empty array of data points
output: Returns the amount of elements in the training set.
updates:
*/
int Data_Point::init_trng_set(Data_Point trng_set[]) {
	printf("Reading dataset file...");
    ifstream input("src/our_dataset.txt");
	if(!input.is_open()) perror("Dataset file read error");
	printf(" Done.\n");

    string line;
    int i = 0, j;
    double magnitude;
	printf("Storing feature test set and normalizing...");
    while(getline(input, line)) {
        magnitude = 0;

        for(j = 0; j < 95; j++)
            magnitude += pow(trng_set[i-1].feat_vecs[j], 2);

        if(magnitude != 0) {
            for(j = 0; j < 95; j++)
                trng_set[i - 1].feat_vecs[j] /= sqrt(magnitude);
        }

        trng_set[i++] = Data_Point(line, 0);
    }
	printf(" Done. Closing dataset file.\n");

	input.close();

    return i;
}

/*
*/
void Data_Point::init_test_set(Data_Point test_set[], int j) {
	GRNN test_ntwk;
	stringstream ab;
	string count, line;
	ab << j;
	ab >> count;

	printf("Adding to test set...");
	string path = "bin/unigrams/output_";
	path += count;
	path += "_unigrams.txt";
	ifstream input(path.c_str());
	if(!input.is_open()) {
		perror("Test read error");
	}

	/*
	double vecs[FEAT_CNT];
	for(i = 0; i < FEAT_CNT; i++)
		vecs[i] = 0;
 */
	getline(input, line);

  test_set[j] = Data_Point(line, 1);

	//test.clsfr = test_ntwk.classify(test_set, test); //CHECK!

	printf(" Done.\n");


}
