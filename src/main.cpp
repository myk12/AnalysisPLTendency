#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include <stdlib.h>
#include "crawl.h"

#define MAX_BUFFER_SIZE (1024*1024)

int main(int argc, char **argv) {
	cout<<"This is a project to analysis Go language tendency"<<endl;

	char buffer[MAX_BUFFER_SIZE] = {0};
	int res = 0;
	string curlcmd;

	curlcmd += "curl -v www.baidu.com";

	crawler myCrawler;
	res = myCrawler.crawling(curlcmd.c_str(), buffer, MAX_BUFFER_SIZE);
	if (res >= 0) {
		cout<<buffer<<endl;
	}else {
		cout<<"Crawling error with cmd :"<<curlcmd<<endl;
	}
}
