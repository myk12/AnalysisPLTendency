#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <fstream>
using namespace std;

#include <stdlib.h>
#include <assert.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include "crawl.h"
#include "utils.h"

#define MAX_BUFFER_SIZE (1024*1024)
#define REPO_QUANTITY_PER_YEAR	(1000)
#define REPO_STARS_LIMIT	("100")


void randomSerachReposForYears(vector<string> years);
void testfunc();

int main(int argc, char **argv) {
	cout<<"This is a project to analysis Go language tendency"<<endl;
	vector<string> searchYears = {"2009", "2010", "2011", "2012",
					"2013", "2014", "2015", "2016",
					"2017", "2018", "2019", "2020", 
					"2021"};

	cout<<"is here"<<endl;
	thread tSearchYear(randomSerachReposForYears, searchYears);

	tSearchYear.join();
	return 0;
}

void testfunc(){
	cout<<"================== randomSerachReposForYears =================="<<endl;
	char buff[MAX_BUFFER_SIZE] = {0};
	crawler mycrawler;
	cout<<"hello world"<<endl;
}


/*
 * function:	randomSerachReposForYears
 * author:	mayuke
 * 
 * brief:	this function can search Github repositories which created from
 *		YEAR_START to YEAR_END and save the result to data directory
 */
void randomSerachReposForYears(vector<string> years){
	cout<<"================== randomSerachReposForYears =================="<<endl;
	char buff[MAX_BUFFER_SIZE] = {0};
	int n = years.size();
	crawler mycrawler;

	// create data directory in current path
	mkdir("data", S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	
	for (auto y:years) {
		cout<<"process year:"<<y<<endl;
		ofstream fd;
		string filename, query;
		int items = 0;

		filename = "./data/" + y + ".dat";
		fd.open(filename);

		//query = UrlEncode("?q=created:" + y + "-01-01.." + y + "-12-31  stars:>=" + REPO_STARS_LIMIT);
		query = "?q=created:" + y + "-01-01.." + y + "-12-31"; //%20stars:>=" + REPO_STARS_LIMIT;
		cout<<"query: "<<query<<endl;
		
		memset(buff, 0, sizeof(buff));
		mycrawler.searchYears(query, REPO_QUANTITY_PER_YEAR, buff, MAX_BUFFER_SIZE);
		
		fd<<buff<<endl;
		
		fd.close();
	}
	
}
		
