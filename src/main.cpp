#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <fstream>
#include <chrono>
using namespace std;
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <stdlib.h>
#include <assert.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include "crawl.h"
#include "utils.h"

#define MAX_BUFFER_SIZE		 (1024*1024)
#define REPO_STARS_LIMIT	("100")
#define SAMPLE_QUANTITY 	(1000)

#define GITHUB_SEARCH_CYCLE (7)

void randomSerachReposForYears(vector<string> years, vector<string> keys);
vector<string> getMonthsOfYear(string y);

int main(int argc, char **argv) {
	cout<<"This is a project to analysis Go language tendency"<<endl;
	vector<string> searchYears = {"2009", "2010", "2011", "2012",
					"2013", "2014", "2015", "2016",
					"2017", "2018", "2019", "2020", 
					"2021"};
	vector<string> searchKeys = {"id", "html_url", "language"};

	thread tSearchYear(randomSerachReposForYears, searchYears, searchKeys);

	tSearchYear.join();
	return 0;
}

/*
 * function:	randomSerachReposForYears
 * author:	mayuke
 * 
 * brief:	this function can search Github repositories which created from
 *		YEAR_START to YEAR_END and save the result to data directory
 */
void randomSerachReposForYears(vector<string> years, vector<string> keys){
	int n = years.size();
	crawler mycrawler(GITHUB_SEARCH_CYCLE);

	// create data directory in current path
	mkdir("data", S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	
	for (auto y:years) {
		string filename, query;
		vector<string> months;
		ofstream fd;
		
		// create and open file to save data
		filename = "./data/" + y + ".dat";
		fd.open(filename);

		// gen every month query
		months = getMonthsOfYear(y);
		for (auto mon:months) {
			string ans;
			query = "?q=created:"+mon;
			query += "&page=16&per_page=100\"";

			while (mycrawler.searchYears(query, keys, ans) <= 0);

			fd<<ans<<endl;
		}

		fd.close();
	}
}

/*
 * function:	getMonthsOfYear
 * author:		mayuke
 * 
 * brief:		this function return 12 months of input year
 *
 */
vector<string> getMonthsOfYear(string y) {
	vector<string> ans;
	ans.emplace_back(y + "-01-01.." + y + "-02-01");
	ans.emplace_back(y + "-02-02.." + y + "-03-01");
	ans.emplace_back(y + "-03-02.." + y + "-04-01");
	ans.emplace_back(y + "-04-02.." + y + "-05-01");
	ans.emplace_back(y + "-05-02.." + y + "-06-01");
	ans.emplace_back(y + "-06-02.." + y + "-07-01");
	ans.emplace_back(y + "-07-02.." + y + "-08-01");
	ans.emplace_back(y + "-08-02.." + y + "-09-01");
	ans.emplace_back(y + "-09-02.." + y + "-10-01");
	ans.emplace_back(y + "-10-02.." + y + "-11-01");
	ans.emplace_back(y + "-11-02.." + y + "-12-01");
	ans.emplace_back(y + "-12-02.." + y + "-12-31");
	return ans;
}
