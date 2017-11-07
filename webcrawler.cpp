#include "webcrawler.h"
#include "datapoint.h"

/* WebCrawler constructor
MAC 11/5/17
Takes in the pre-defined root url and starts a stack to crawl the web with.

inputs: root url
outputs: n/a
updates:
*/
WebCrawler::WebCrawler(string root, int depth) {
	root_url = root;
	dpth_max = depth;
	
	web_node start_stack;
	start_stack.url = root;
	start_stack.depth = 0;
	
	web_stack.push(start_stack);
}

/* Initialize Weights
MAC 11/5/17
Reads in weights from a text file and stores them to the array of weights.

inputs:
outputs: number of weights in the array
updates:
*/
int WebCrawler::init_weights() {
	ifstream input("weights.txt");
	string line;
	int j;
	
	while(getline(input, line))
		weights[j++] = atof(line.c_str());
	
	return j;
}

/*
*/
int WebCrawler::find_children(web_node parent, int j) {
	char javaCall[1000] = "";
	char url[1000] = "";
	int chld_cnt = 0;
	web_node new_chld;
	
	string hyperlink = "", child_url = "", i = "";
	stringstream ab;
	ab << j;
	ab >> i;
	//char *k = new char[i.size()+1];
	//strcpy(k, i.c_str());
	
	// Make java call to save output file copy of HTML source code
	strcpy(javaCall, "java getWebPage ");
	strcat(javaCall, web_stack.top().url.c_str());
	strcat(javaCall, " > bin/html_source/output_");
	strcat(javaCall, i.c_str());
	strcat(javaCall, ".txt");
	puts(javaCall);
	system(javaCall);
	
	// Remove parent from stack
	web_stack.pop();
	
	string page = "bin/html_source/output_";
	page += i;
	page += ".txt";
	cout << "Storing HTML source in: /bin/html_source" << page << endl;
	ifstream webpage(page.c_str());
	if(!webpage.is_open()) perror("HTML source write error");
	
	while(!webpage.eof()) {
		//webpage >> setw(99) >> url;
		getline(webpage, hyperlink);
		hyperlink.erase(hyperlink.begin(), find_if(hyperlink.begin(), hyperlink.end(), bind1st(not_equal_to<char>(), ' ')));
		
		if(strncmp("<a href", hyperlink.c_str(), 7) == 0) {
			stringstream ss(hyperlink);
			ss.ignore(10, '"');
			getline(ss, child_url, '"');
			
			if((child_url.length() > 4) && (strncmp(child_url.substr(child_url.length() - 3, child_url.length()).c_str(), "pdf", 3) != 0) && (strncmp(child_url.substr(0, 4).c_str(), "http", 4) == 0)) {
				new_chld.url = child_url;
				new_chld.depth = parent.depth + 1;
				web_stack.push(new_chld);
				chld_cnt++;
			}
		}
	}
	
	char_extractor(page, j);
	
	webpage.close();
	
	
	return chld_cnt;
}

/*
*/
void WebCrawler:: char_extractor(string filepath, int j) {
	string new_filepath, count;
	char c;
	double mag = 0, mag_squared = 0;
	int i, k, stored = 0;
	double unigram_count[FEAT_CNT];
	
	stringstream ab;
	ab << j;
	ab >> count;
	
	cout << "\nBegin feature extraction from " << filepath << "...";
	
	// Initialize all counts to 0
	for(i = 0; i < FEAT_CNT; i++)
		unigram_count[i] = 0;
	
	// Count the frequency of each printable character in the HTML source
	ifstream in_text(filepath.c_str());
	if(!in_text.is_open()) perror("Unigram extraction error");
	while(in_text >> noskipws >> c) {
		if(isprint(c)) {
			k = c - 32;
			unigram_count[k]++;
		}
	}
	in_text.close();
	
	// Find magnitude
	for(i = 0; i < FEAT_CNT; i++)
		mag_squared += pow(unigram_count[i], 2);
	
	
	// If feature vector is not zero vector, write to output file
	if(mag_squared > 0) {
		// Magnitude is square-root of sum of squares. Kept as int until after comparison
		mag = sqrt((double) mag_squared);
		cout << "Magnitude is: " << mag << endl;
		
		// Create output stream to file containing normalized feature vector
		new_filepath = "bin/unigrams/output_";
		new_filepath += count;
		new_filepath += "_unigrams.txt";
		ofstream out_text(new_filepath.c_str());
		if(!out_text.is_open()) perror("Normalization write error");
		cout << "Storing normalized unigram to /bin/unigrams/" << new_filepath  << endl;
		
		// First write the index of the output file
		out_text << count;
		
		// Write normalized unigrams/output
		for(i = 0; i < FEAT_CNT; i++) {
			if(mag != 0) out_text << " " << (unigram_count[i] / mag);
			else out_text << " " << unigram_count[i];
		}
		out_text.close();
		
		add_test(new_filepath);
	}
	
	else {
		printf(" Deleting file.\n");
		remove(("bin/html_source/output_" + count + ".txt").c_str());
	}
	
	return;
}

/*
*/
void WebCrawler::add_test(string uni_file) {
	Data_Point test_pnt;
	printf("Adding to test dataset...");
	ifstream input(uni_file.c_str());
	if(!input.is_open()) perror("Unigram read error");
	ofstream output;
	output.open("bin/test_set.txt", ios_base::app | ios_base::out);
	if(!output.is_open()) perror("Test set write error");
	string line;
	int i;
	double mag = 0;
	
	getline(input, line);
	test_pnt = Data_Point(line);
	printf("\n");
	for(i = 0; i < FEAT_CNT; i++) {
		output << test_pnt.feat_vecs[i] << " ";
		printf("%f, ", test_pnt.feat_vecs[i]);
	}
	printf("\n");
	
	output.close();
	input.close();
	
	printf(" Done.");
	
	return;
}

/*
*/
int WebCrawler::crawl(int j) {
	string filepath;
	int depth;
	
	filepath = web_stack.top().url;
	depth = web_stack.top().depth;
	cout << "\n\nURL being expanded is: " << filepath;
	cout << " at depth " << depth << endl;
	
	printf("Searching for children...\n");
	int chld_cnt = 0;
	if(depth < dpth_max) {
		find_children(web_stack.top(), j);
		chld_cnt++;
	}
	web_stack.pop();
	
	return chld_cnt;
}