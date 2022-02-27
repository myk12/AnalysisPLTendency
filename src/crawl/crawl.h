#ifndef __CRAWL_H_
#define __CRAWL_H_

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define CURL_API_HEAD 	"curl -H \"Accept: application/vnd.github.v3+json\" "

#define CURL_SEARCH_REPO	" \"https://api.github.com/search/repositories"
#define CURL_SEARCH_USER	" \"https://api.github.com/search/users"

#define CURL_GET_USER_REPOS " \"https://api.github.com/users/"

class crawler {
private:
	int cycle;
public:
	crawler(int cycle);
	int execcmd(string cmd, char *buffer, int bufflen);
	int search(string query, vector<string>, string &res);
	int getUserRepos(string uesrname, vector<string> infos, string &res);
};	

#endif
