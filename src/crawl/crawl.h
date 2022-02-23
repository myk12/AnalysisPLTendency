#ifndef __CRAWL_H_
#define __CRAWL_H_

#include <iostream>
using namespace std;

#define CRUL_GITHUB_API_HEAD ("curl -H \"Accept: application/vnd.github.v3+json\" ")

class crawler {
public:
	int crawling(const char *cmd, char *buffer, int bufflen);

	int searchYears(string query, 
			int items,
			char *buffer,
			int bufflen);
};	

#endif
