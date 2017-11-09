#ifndef WEB_CRLR
#define WEB_CRLR

#include "datapoint.h"
#include <stack>
#include <cstdlib>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct web_node {
	string url;
	int depth;
};

class WebCrawler {
	private:
	int find_children(web_node parent, int j);
	void char_extractor(string filepath, int j);
	
	public:
	stack<web_node> web_stack;
	string root_url;
	double weights[POPULATION];
	int dpth_max;
	
	WebCrawler(string root, int depth);
	int init_weights();
	int crawl(int j);
	
};

#endif