# 1. Intro
This project programed by C++ is a client that can automatically crawl
users' infomation or Github repositories from Github using Github API.

# 2. Build

## 2.1 enviroment

**machine:**
- Ubuntu 20.04 (Linux ubuntu 5.11.0-44-generic #48~20.04.2-Ubuntu SMP)

**tools and lib-dev**
+ cmake version 3.10+
+ gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
+ network connection to Github

## 2.2 code
open a terminal and run the command：
`git clone https://github.com/myk12/AnalysisPLTendency.git`

## 2.3 compile

`cd ./AnalysisPLTendency`

`mkdir -p build & cd build`

`cmake ../src`

`cmake --build .`

## 2.4 run
After you have built the project, there will be an executable file named `GoAnalysis` under
the "build" directory.

run 
`./GoAnalysis`

# 3. Output

This client automatically crawls data from Github, it contains two threads that one thread randomly 
crawls tens of thousands of repositories from 2009 to 2021 (over a thousand per year on average) and 
stores the results in directory ./build/data/year_repos. Another thread crawls the user information 
that meets the search criteria and stores the repositories data they own in directory ./build/data/user_repos.

# 4. Others

## 4.1 Github API rate limit

Due to the Github API [rate limit](https://docs.github.com/en/rest/reference/rate-limit), the client's 
search thread sleeps for a certain amount of time before each search to prevent exceeding the speed limit.

Each time the server returns a rate limit prompt, the search thread automatically doubles the sleep time, 
and the maximum sleep time is 1024 times.

## 4.2 Data set size

There are a number of preprocessor macros in the source code that can be modified to adjust the amount of 
data retrieved from the server, which may result in long program execution if a large amount of data is 
specified.
