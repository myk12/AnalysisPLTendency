//crawl data from github
#include <iostream>
using namespace std;
#include <stdlib.h>
#include "crawl.h"

int crawler::crawling(const char *cmd, char *buffer, int bufflen) {
	if (!cmd) {
		return -1;
	}
	cout<<"recv cmd :"<<cmd<<endl;

	system(cmd);
	return 0;
}
