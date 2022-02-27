//crawl data from github
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "crawl.h"


#define MAX_BUFF_LEN (1024*1024)

#define MAX_WAIT_TIME (1024)

crawler::crawler(int c){
	cycle = c;
}

/*
 * function:	execcmd
 * author  :	mayuke
 * data    :	2022-02-26
 * 
 * brief   :	this funtion execute Github API command and
 *				put the result into the buffer
 *
 */
int crawler::execcmd(string cmd, char *buffer, int bufflen) {
	if (!cmd.size() || !buffer) {
		return -1;
	}

	char	*cur 		= nullptr;
	int		len  		= 0;
	int 	limit		= 1;
	FILE	*cmdPipe	= nullptr;

	do {	
		// read result from pipe
		cur = buffer;
		len = bufflen;
		memset(buffer, 0, sizeof(buffer));

		// beacuse of Github Search Rate limit, sleep for a while for every search cycle
		this_thread::sleep_for(std::chrono::seconds(cycle*limit));
	
		cmdPipe = popen(cmd.c_str(), "r");
		assert(cmdPipe);		
		cout<<"exec cmd:"<<cmd<<endl;

		while (fgets(cur, len, cmdPipe) != NULL) {
			len = bufflen - strlen(buffer);
			cur = buffer + strlen(buffer);
		}
		
		// exponetial growth in case of frequently failing;
		limit = min(limit*2, MAX_WAIT_TIME);

		cout<<buffer<<endl;
	}while (strstr(buffer, "API rate limit exceeded"));  //API rate limit messge
	
	pclose(cmdPipe);

	return bufflen - len;
}

/*
 * function:	getUserRepos()
 * author  :	mayuke
 * date    :	2020-02-26
 *
 * brief   :	this function get users' repositories using Github API
 *
 * param[in]:	username
 */
int crawler::getUserRepos(string username, vector<string> keys, string &res){
	char buf[MAX_BUFF_LEN] 	= {0};
	char *cur 				= buf;
	int  ret				= 0;
	string crulCmd			= CURL_API_HEAD CURL_GET_USER_REPOS;
	json jsonRes;

	// construct query
	crulCmd += (username + "/repos\"");

	// execute command and get result
	ret = execcmd(crulCmd, buf, sizeof(buf) - 1);
	if (ret < 0) return -1;

	cout<<"get user repos"<<endl<<buf<<endl;
	// extract user repositories
	for (auto item:json::parse(buf)) {
		json j;
		for (auto k:keys) {
			j[k] = item[k];	
		}
		jsonRes.emplace_back(j);
	}
	
	res = jsonRes.dump();
	return jsonRes.size();
}

/* 
 * function: 	search()
 * author  :	mayuke
 * 
 * brief   :	this function launch search interface to search from Github using Github API
 *
 * param[in]:	query	search query
 * param[in]:	keys	result keys
 *
 */
int crawler::search(string query, vector<string> keys, string &res) 
{
	char 	buf[MAX_BUFF_LEN] 	= {0};
	char 	*cur 				= buf;
	int 	ret					= 0;
	string 	crulCmd 			= CURL_API_HEAD+ query;	
	json	jsonRes;
	
	// execute command and get result
	ret = execcmd(crulCmd, buf, sizeof(buf) - 1);
	if (ret < 0) return -1;

	cout<<"get buf"<<endl<<buf<<endl;

	// extract vlaues of keys
	try {
		auto items = json::parse(buf)["items"];
		for (auto it:items) {
			json j;
			for (auto k:keys) {
				j[k] = it[k];
			}

			jsonRes.emplace_back(j);
		}			
	}catch(...) {
		return -1;
	}
	res = jsonRes.dump();
	
	return jsonRes.size();
}

