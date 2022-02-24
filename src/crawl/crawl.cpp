//crawl data from github
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "crawl.h"

#define CRUL_GITHUB_API_HEAD 	"curl -H \"Accept: application/vnd.github.v3+json\" "
#define CRUL_GITHUB_SEARCH_REPO	" \"https://api.github.com/search/repositories"

#define SERACH_RES_BUFF_LEN (1024*1024)

crawler::crawler(int c){
	cycle = c;
}

int crawler::crawling(const char *cmd, char *buffer, int bufflen) {
	if (!cmd) {
		return -1;
	}
	cout<<"recv cmd :"<<cmd<<endl;

	system(cmd);
	return 0;
}

/* 
 * function: 	searchYears()
 * author:	mayuke
 * 
 * brief:	this function search query from Github using Github API
 *
 * param[in]:	query	search query
 * param[in]:	keys	result keys
 *
 */
int crawler::searchYears(string query, vector<string> keys, string &res) 
{
	// beacuse of Github Search Rate limit, sleep for a while for every search cycle
	this_thread::sleep_for(std::chrono::seconds(cycle));

	char buf[SERACH_RES_BUFF_LEN] = {0};
	char *cur = buf;
	int len = SERACH_RES_BUFF_LEN;
	int cnt = 0;
	string crulCmd;
	FILE *cmdPipe;

	crulCmd = ( CRUL_GITHUB_API_HEAD CRUL_GITHUB_SEARCH_REPO ) + query;	
	cout<<crulCmd<<endl;
	cmdPipe = popen(crulCmd.c_str(), "r");

	if (!cmdPipe) return -1;
	
	// read result from pipe
	while (fgets(cur, len, cmdPipe) != NULL) {
		len = SERACH_RES_BUFF_LEN - strlen(buf);
		cur = buf + strlen(buf);
	}

	// extract vlaues of keys
	try {
		auto items = json::parse(buf)["items"];
		for (auto it:items) {
			json j;
			for (auto k:keys) {
				j[k] = it[k];
			}

			res += (j.dump() + '\n');
			cnt++;
		}			
	}catch(...) {
		pclose(cmdPipe);
		return 0;
	}
	
	pclose(cmdPipe);
	return cnt;
}
