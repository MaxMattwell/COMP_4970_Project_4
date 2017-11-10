#include "./src/webcrawler.h"
#include "./src/datapoint.h"
#include "./src/grnn.h"

#define ROOT_URL "http://www.auburn.edu"
#define MAX_DEPTH 6
#define SIGMA 0.1237

using namespace std;

double calculateSD(double classifications[]);

int main() {
	// Instantiate and Initialize arrays for the training set and cluster points
  Data_Point trng_set[TRAINING_SET];
	Data_Point np = Data_Point();


	printf("\n--------------------------------------------------------------------------------\n");
	printf("Build training set:\n");
    int act_pop = np.init_trng_set(trng_set); // Actual population of the set

	printf("Training elements found: %i\n\n", act_pop);
/*
  for(int j = 0; j < 595; j++) {
    for (int k = 0; k < 95; k++) {
        cout << trng_set[j].feat_vecs[k];
        cout << " ";
    }
      cout << endl;
  }
  */

	/*
	printf("--------------------------------------------------------------------------------\n\n");
	printf("Classify training set...");
  */
	int i, true_pos = 0, true_neg = 0, false_pos = 0, false_neg = 0;
	double acc = 0;
	GRNN trng_ntwk = GRNN(SIGMA);

	printf("--------------------------------------------------------------------------------\n\n");

	printf("Loading web crawler...");
	WebCrawler web_crwl = WebCrawler(ROOT_URL, MAX_DEPTH);
	web_crwl.init_weights();

  cout << "Weights: " << endl;
  for (int i = 0; i < 95; i++) {
    cout << web_crwl.weights[i] << " ";
  }
  cout << endl;

	printf(" Done.\n");

	Data_Point test_set[POPULATION];
  int expanded_webpages = 0;
  int update_expanded_webpages = 0;
	printf("Crawling...\n");
	//int num_test_elements = 0;
	while (expanded_webpages < 200) {
		if(web_crwl.web_stack.empty()) break;
		update_expanded_webpages = web_crwl.crawl(expanded_webpages);
    //cout << "deleted output file or not: " << update_expanded_webpages << endl;
    if (update_expanded_webpages == 0) {
		    np.init_test_set(test_set, expanded_webpages);
        expanded_webpages++;
    }
	}

	printf("Done. %i test elements stored\n", expanded_webpages);

  cout << "Test set elements: " << endl;

  for (int i = 0; i < POPULATION; i++) {
    cout << "Test set element " << i << endl;
    for (int j = 0; j < FEAT_CNT; j++) {
    cout << test_set[i].feat_vecs[j] << " ";
    }
    cout << endl;
}

	printf("--------------------------------------------------------------------------------\n\n");
/*
	printf("Classify test set:\n");
	GRNN test_ntwk = GRNN();
	for(i = 0; i < test_pop; i++)
		test_set[i].clsfr = trng_ntwk.classify(trng_set, test_set[i]);
	for(i = 0; i < test_pop; i++){
		if(test_set[i].pnt_id >= 0)
			printf("%i: %f...%f, %f, %f, %f, %f\n", test_set[i].pnt_id, test_set[i].clsfr, test_set[i].feat_vecs[0], test_set[i].feat_vecs[1], test_set[i].feat_vecs[2], test_set[i].feat_vecs[3], test_set[i].feat_vecs[4]);
	}
	printf(" Done.\n");
*/

	//printf("Totals - \n\t[1.0,0.5): ,[0.5, 0.0): ,[0.0, -0.5), [-0.5, 1.0]\n");
	//printf("Balancing training set...\n\n");

  printf("Classify test set:\n");
  double classification = 0, total = 0, first_hundred_total = 0, last_hundred_total = 0;
  double classification_array[200] = {};
  double first_hundred_array[100] = {};
  double last_hundred_array[100] = {};
  int interval = 0;
  for (int i = 0; i < POPULATION; i++) {
    classification = trng_ntwk.classify(trng_set, test_set[i], web_crwl.weights);
    classification_array[i] = classification;
    total += classification;
    if (i <= 99) {
      first_hundred_total += classification;
      first_hundred_array[i] = classification;
    }
    if (i >= 100) {
      last_hundred_total += classification;
      last_hundred_array[i] = classification;
    }
    cout << "Classification for test instance " << i << ": " << classification << endl;
    if ((classification == -0.015) || (classification == 0.015) || ((classification > -0.015) && (classification < 0.015))) {
        interval++;
    }
  }

  // Write classifications to file
  ofstream out_text("src/classifications_without_SSGA.txt", ofstream::app);
  for(i = 0; i < POPULATION; i++) {
    out_text << classification_array[i] << endl;
  }
  out_text.close();

  cout << "The number of pages that received a GRNN output within the interval [-0.015 to 0.015] inclusive was " << interval << endl;
  cout << "The mean of the 200 outputs is " << (total / POPULATION) << endl;
  cout << "The standard deviation of the 200 outputs is " << calculateSD(classification_array) << endl;
  cout << "The mean of the first 100 outputs is " << (first_hundred_total / (POPULATION / 2)) << endl;
  cout << "The standard deviation of the first 100 outputs is " << calculateSD(first_hundred_array) << endl;
  cout << "The mean of the last 100 outputs is " << (last_hundred_total / (POPULATION / 2)) << endl;
  cout << "The standard deviation of the last 100 outputs is " << calculateSD(last_hundred_array) << endl;

	printf("--------------------------------------------------------------------------------\n\n");

	return 0;
}

double calculateSD(double classifications[])
{
    double sum = 0, mean = 0, standardDeviation = 0;

    int i;

    for(i = 0; i < 200; i++)
    {
        sum += classifications[i];
    }

    mean = sum/200;

    for(i = 0; i < 200; i++)
        standardDeviation += pow(classifications[i] - mean, 2);

    return sqrt(standardDeviation / 200);
}
