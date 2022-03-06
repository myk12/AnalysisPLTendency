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

#define MAX_BUFFER_SIZE		 	(1024*1024)
#define REPO_STARS_LIMIT		("100")
#define SAMPLE_QUANTITY 		(1000)

#define SEARCH_THREAD_NUM 		(1)
#define SEARCH_CYCLE 			(SEARCH_THREAD_NUM * 7)

#define TEST_TH1
//#define TEST_TH2
//#define TEST_TH3

void randomSerachReposForYears(vector<string> years, vector<string> keys);
void specifySearchReposForUsers(vector<string> keys, vector<string> infos);
void randomSearchReposForGolang(vector<string> years, vector<string> keys);
vector<string> getMonthsOfYear(string y);

int main(int argc, char **argv) {
	cout<<"This is a project to analysis Go language tendency"<<endl;

	// create data directory in current path
	mkdir("data", S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);

	vector<string> searchYears 	= {	"2009", "2010", "2011", "2012",
									"2013", "2014", "2015", "2016",
									"2017", "2018", "2019", "2020",
									"2021" };

	vector<string> searchKeys 	= { "id", "html_url", "language" , "stargazers_count", "watchers_count", "forks_count"};

	vector<string> searchUser	= { "login" };
	vector<string> UserRepoInfo = { "id", "full_name", "language"};

	vector<string> searchGoRepo = { "id", "full_name", "language", "created_at", "topics", "description", "size"};
#ifdef TEST_TH1
	thread tSearchYear(randomSerachReposForYears, searchYears, searchKeys);
#endif
#ifdef TEST_TH2
	thread tSearchUser(specifySearchReposForUsers, searchUser, UserRepoInfo);
#endif
#ifdef TEST_TH3
	vector<string> searchGoYears(searchYears.begin() + 8, searchYears.end());
	thread tSearchGoRepo(randomSearchReposForGolang, searchGoYears, searchGoRepo);
#endif

#ifdef TEST_TH1
	tSearchYear.join();
#endif
#ifdef TEST_TH2
	tSearchUser.join();
#endif
#ifdef TEST_TH3
	tSearchGoRepo.join();
#endif
	return 0;
}

void randomSearchReposForGolang(vector<string> years, vector<string> keys) {
	string dataDir = "data/go_repos/";
	crawler mycrawler(SEARCH_CYCLE);

	// create daat directory in current path
	system("rm -rf data/go_repos");
	mkdir(dataDir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	
	// search repos of Go lang
	for (auto y:years) {
		vector<string>	months;
		string 			filename;
		ofstream 		fd;
		
		// create and open file to save data
		filename = dataDir + y + ".dat";
		fd.open(filename);
		assert(fd);

		// gen every month query
		months = getMonthsOfYear(y);
		for (auto mon:months) {
			string ans;
			string query;
			query += CURL_SEARCH_REPO;
			query += "?q=created:"+ mon + "%20language:Go";
			query += "&page=8&per_page=100\"";

			while (mycrawler.search(query, keys, ans) < 0);

			fd<<ans<<endl;
		}

		fd.close();
	}
}
	

/*
 * function:	specifySearchReposForUsers
 * author  :	mayuke
 *
 * brief   :	this fucntion can search users from Github and collect 
 * 				their repos data that will be processed and saved to files.
 */
void specifySearchReposForUsers(vector<string> keys, vector<string> infos){
	
	string dataDir = "data/user_repos/";

	// create data directory in current path
	system("rm -rf data/user_repos");
	mkdir(dataDir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);

	// search users from github which have repos
	unordered_map<string, string>	userRepos;
	string 							query, ans;
	json 							jsonUsers;
	crawler 						mycrawler(SEARCH_CYCLE);

	query += CURL_SEARCH_USER;
	query += "?q=repos:>=64&followers:>=1024&per_page=100\"";

	while (mycrawler.search(query, keys, ans) <= 0);
	
	cout<<"get user rpos"<<endl<<ans<<endl;
	
	for (auto item:json::parse(ans)) {
		ofstream	fd;
		string 		filename, res, username = item["login"];

		filename = dataDir + username + ".dat";
		fd.open(filename);
		assert(fd);

		// get user repos
		while (mycrawler.getUserRepos(username, infos, res) <= 0);
		userRepos[username] = res;

		fd<<res<<endl;
		fd.close();
	}
	
}

/*
 * function:	randomSerachReposForYears
 * author  :	mayuke
 * 
 * brief   :	this function can search Github repositories which created from
 *		YEAR_START to YEAR_END and save the result to data directory
 */
void randomSerachReposForYears(vector<string> years, vector<string> keys){
	string	dataDir = "data/year_repos/";
	crawler mycrawler(SEARCH_CYCLE);

	// create data directory in current path
	mkdir(dataDir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	
	for (auto y:years) {
		vector<string>	months;
		string 			filename;
		ofstream 		fd;
		
		// create and open file to save data
		filename = dataDir + y + ".dat";
		fd.open(filename);
		assert(fd);

		// gen every month query
		months = getMonthsOfYear(y);
		for (auto mon:months) {
			string ans;
			string query;
			query += CURL_SEARCH_REPO;
			query += "?q=created:"+mon;
			query += "&page=8&per_page=100\"";

			while (mycrawler.search(query, keys, ans) <= 0);

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
