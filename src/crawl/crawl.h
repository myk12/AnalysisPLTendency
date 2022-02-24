#ifndef __CRAWL_H_
#define __CRAWL_H_

#include <iostream>
#include <vector>
#include <string>
using namespace std;


class crawler {
private:
	int cycle;
public:
	crawler(int cycle);
	int crawling(const char *cmd, char *buffer, int bufflen);
	int searchYears(string query, vector<string>, string &res);
};	

#endif
