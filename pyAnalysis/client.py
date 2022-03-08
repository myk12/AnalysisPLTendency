import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import json
import requests
import time

datadir = "data/"
godata = "go_repos/"
outputdir = "./result/"
resfile = outputdir + "commits.dat"

def readFromFile(filename):
	fd = open(filename, "r")
	lines = fd.readlines()
	fd.close()

	return lines

def getAllGoRepos(path, files):
	
	repos = pd.Series([])
	for fl in files:
		filecontent = readFromFile(path + fl)

		# count the total number of Go project
		for content in filecontent:
			#each line represent one month in a year
			rp = pd.DataFrame(json.loads(content))['full_name']
			try:
				repos = pd.concat([repos, rp])
			except KeyError:
				repos = repos
	return repos

def getReposCommits(full_name):
	'''
	this function get repos's commit info using Github API
	'''
	timelist = []
	url = "https://api.github.com/repos/" + full_name + "/commits"
	header = {"Authorization":"token "+"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"}

	while True:
		print(url)
		res = requests.get(url, headers=header)
		if res.status_code ==  200:
			break
		else:
			#sleep  untils rate limit is lifted
			count = 0
			while True:
				count+=1
				url_rate = "https://api.github.com/rate_limit"	
				res = requests.get(url_rate, headers=header)
				if res.status_code == 200 and (res.json())['rate']['remaining'] > 0:
					break
				else:
					print("No.[%d] sleep for rate limit" %count)	
					time.sleep(60)
	
	res_json = res.json()
	for item in res_json:
		timelist.append(item["commit"]['committer']['date'])
	return timelist

def savelisttofile(timelist):
	print("saving list time to file")
	fd = open(resfile, "a")
	for time in timelist:
		fd.write(time + '\n')
	fd.close()
	
def requestReposCommits(repos):
	
	committime = []
	print("================= START Crawling ================")
	repos_num = len(repos)
	count = 0
	print("[Total repos]: %d" %(repos_num))
	for idx, val in repos.items():
		if idx%100:
			committime += getReposCommits(val)
		else:
			savelisttofile(committime)
			count += 100
			print("------|%d/%d|------" %(count , repos_num))
			committime = []
	savelisttofile(committime)

def readDateDictFromFile():
	items = readFromFile(resfile)
	
	
	
	

def main():
	godataDir = datadir + godata
	godatafiles = os.listdir(godataDir)
	
	#get all repos' full_name list
	reposeries = getAllGoRepos(godataDir, godatafiles)

	#get all repos' commit info	and save to file
	requestReposCommits(reposeries)

	#read commit info from file and generate map graph
	
 
if __name__ == "__main__":
	main()	
