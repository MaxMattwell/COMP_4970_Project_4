#include "./src/webcrawler.h"
#include "./src/datapoint.h"
#include "./src/grnn.h"

#define ROOT_URL "http://www.auburn.edu"
#define MAX_DEPTH 6
#define SIGMA 0.1237


int main() {
	// Instantiate and Initialize arrays for the training set and cluster points
    Data_Point trng_set[POPULATION];
	Data_Point np = Data_Point();
    
	
	printf("\n--------------------------------------------------------------------------------\n");
	printf("Build training set:\n");
    int act_pop = np.init_trng_set(trng_set); // Actual population of the set
	
	printf("Training elements found: %i\n\n", act_pop);
	
	printf("--------------------------------------------------------------------------------\n\n");
	printf("Classify training set...");
	int i, true_pos = 0, true_neg = 0, false_pos = 0, false_neg = 0;
	double acc = 0;
	
	GRNN nurl_ntwk = GRNN(trng_set, SIGMA);
	
	printf(" Done.\n");
	//for(i = 0; i < 595; i++)
	//	printf("%i: %f\n", i, nurl_ntwk.pred_set[i].clsfr);
	
	for(i = 0; i < act_pop; i++) {
		if(nurl_ntwk.pred_set[i].clsfr < 0 && trng_set[i].clsfr < 0) true_neg++;
		else if(nurl_ntwk.pred_set[i].clsfr > 0 && trng_set[i].clsfr > 0) true_pos++;
		else if(nurl_ntwk.pred_set[i].clsfr < 0 && trng_set[i].clsfr > 0) false_neg++;
		else false_pos++;
	}
	
	acc = true_pos + true_neg;
	acc /= act_pop;
	
	printf("Classification accuracy: %f\n", acc);
	printf("True Pos: %i, True Neg: %i,\n", true_pos, true_neg);
	printf("False pos: %i, False neg: %i\n\n", false_pos, false_neg);
	
	printf("--------------------------------------------------------------------------------\n\n");
	
	printf("Loading web crawler...");
	WebCrawler web_crwl = WebCrawler(ROOT_URL, MAX_DEPTH);
	web_crwl.init_weights();
	printf(" Done.\n");
	
	double mgtd;
	
	printf("Crawling...\n");
	int j = 0, x;
	Data_Point data;
	for(i = 0; i < (POPULATION - act_pop); i++) {
		if(web_crwl.web_stack.empty()) break;
		
		x += web_crwl.crawl(j++);
	}
	
	printf("Done. Crawled %i pages, %i normalized unigrams stored\n", (j - 1), x);
	
	printf("--------------------------------------------------------------------------------\n\n");
	
	printf("Build test set:");
	Data_Point test_set[POPULATION];
	int test_cnt = np.init_test_set(test_set);
	printf("Done\n");
	for(i = 0; i < 20; i++) {
        cout << "i = " << i << endl;
        for(j = 0; j < FEAT_CNT; j++) {
            //cout << "j = " << j << endl;
			printf("%f, ", test_set[i].feat_vecs[j]);
        }
		printf("\n\n");
	}
	//printf("Classify test set:\n");
	//printf("Totals - \n\t[1.0,0.5): ,[0.5, 0.0): ,[0.0, -0.5), [-0.5, 1.0]\n");
	//printf("Balancing training set...\n\n");
	
	printf("--------------------------------------------------------------------------------\n\n");
	
	return 0;
}
