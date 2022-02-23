//crawl data from github
#include <iostream>
using namespace std;
#include <stdlib.h>
#include "crawl.h"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


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
 * param[in]:	items	quantity of search items
 * param[in]:	buffer	buffer to store the result
 * param[in]:	bufflen	length of buffer
 *
 */
int crawler::searchYears(string query,
		int items,
		char *buffer,
		int bufflen) 
{

	string crulCmd;
	FILE *cmdPipe;
	crulCmd = CRUL_GITHUB_API_HEAD + query;	
	cout<<crulCmd<<endl;
	cmdPipe = popen(crulCmd.c_str(), "r");

	if (!cmdPipe) return -1;
	
	fgets(buffer, bufflen, cmdPipe);

	/*
	while (fgets(cur, len, cmdPipe) != NULL) {
		len = bufflen - strlen(buffer);
		cur = buffer + strlen(buffer);
	}	
	*/
	
	pclose(cmdPipe);
	return strlen(buffer);
}
