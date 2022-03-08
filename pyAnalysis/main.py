import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import json
import time
import math
from datetime import datetime
from wordcloud import WordCloud as wcd

datadir = "data/"
yeardata = "year_repos/"
godata = "go_repos/"
outputdir = "./result/"

def logValue(val, base=2):
	if (val <= 0):
		return 0
	return math.log(val, base) 

def timestr2timestamp(timestr):
	'''
	This function converys timestring to UNIX timestamp
	'''
	if pd.isnull(timestr):
		return 0
	dt = datetime.strptime(str(timestr), '%Y-%m-%dT%H:%M:%SZ')
	return dt.timestamp()
	

def readFromFile(filename):
	lines = []
	if (not filename):
		return lines
	
	print("read files : %s" %filename)
	fd = open(filename, "r")
	lines = fd.readlines()
	fd.close()

	return lines

def generateWordCloud(txtfile, path="/tmp/", name="wordcloud.png"):
	text = open(txtfile).read()
	wordcloud = wcd(background_color="white").generate(text)
	plt.imshow(wordcloud, interpolation='bilinear')
	plt.axis("off")
	plt.savefig(outputdir + name, dpi=800)
	plt.show()

def analysisGoProj(path, files):
	'''
	this function analysis the data of Go project from Github
	'''
	files.sort() # rank with year	
	
	create_time = []
	proj_size = []
	dfall = pd.DataFrame()

	#each file represents one year data
	for fl in files:
		filename = "/tmp/" + fl

		filecontent = readFromFile(path + fl)

		df = pd.DataFrame()
		for cont in filecontent:
			dft = pd.DataFrame(json.loads(cont))
			df = pd.concat([df, dft], sort = False)

		try:
			des = df["description"].values
			np.savetxt(filename, des[des != None], fmt = "%s", delimiter = ' ')
		except KeyError:
			print("empty file")
		
		dfall = pd.concat([dfall, df], sort = False)

		#generate wordcloud
		#generateWordCloud(filename)

	ct = dfall["created_at"].apply(timestr2timestamp)
	size = dfall["size"].apply(logValue)
	result = pd.concat([ct, size], axis=1)
	result.plot.scatter(x="created_at", y="size", color="DarkGreen")
	plt.savefig(outputdir + "scatter.png", dpi=800)
	plt.show()
	
	#generate wordcloud
	descpt = dfall["desctiption"].values
	np.savetxt("/tmp/wordcloudall.txt", des[des != None], fmt = "%s", delimiter = " ")
	generateWordCloud("/tmp/wordcloudall.txt", outputdir, "wordcloudall.png")

def analysisTimeline(path, files, lang):
	'''
	this function analysis the data of number of Go project every year form 2009 to	2021
	and generate a bar char to vasulize the result
	'''
	
	#sort by year
	files.sort()
	columns_name = ["repos_count", "stargazers_count", "watchers_count", "forks_count"]

	year_repos_list = []
	index_list = []

	for fl in files:
		#each year represents a year
		dffile = pd.DataFrame()

		#read file
		filecontent = readFromFile(path + fl)

		# count the total number of Go project
		for content in filecontent:
			#each line represent one month in a year
			try:
				dfline = pd.DataFrame(json.loads(content))
				dfline = dfline.loc[dfline["language"] == lang]
			except KeyError:
				dfline = pd.DataFrame()
			
			dffile = pd.concat([dffile, dfline])
		
		year_repos_list.append([dffile.shape[0],\
								dffile[columns_name[1]].sum(),\
								dffile[columns_name[2]].sum(),\
								dffile[columns_name[3]].sum()])

		index_list.append(fl.split('.')[0])

	return pd.DataFrame(year_repos_list, columns = columns_name, index = index_list)

def reposChangesForYears(path, files):
	dataDict = {}
	year_list = []
	langList = ["Go", "C++", "Java", "Python"]
	
	#get year list
	files.sort()
	for fl in files:
		year_list.append(fl.split('.')[0])

	fig = plt.figure(figsize=(8, 6))

	for lang in langList:
		dflang = analysisTimeline(path, files, lang)
		dataDict[lang] = dflang
		print("=================================== %s ======================================" %lang)
		print(dataDict[lang])
		plt.plot(year_list, dflang['repos_count'], label=lang)

	plt.xlabel("years")
	plt.ylabel("repos_count")
	plt.legend()
	plt.savefig(outputdir + "yearsChange.png", dpi=800)
	plt.show()
		

	for lang in langList:
		dflang = analysisTimeline(path, files, lang)
		dflang['hot'] = dflang["repos_count"]*(dflang["stargazers_count"].apply(logValue, 2)\
											+ dflang["watchers_count"].apply(logValue, 2)\
											+ dflang["forks_count"].apply(logValue, 2))
		plt.plot(year_list, dflang['hot'], label=lang)

	plt.xlabel("years")
	plt.ylabel("repos_count")
	plt.legend()
	plt.savefig(outputdir + "yearsChange_modifed.png", dpi=800)
	plt.show()

def main():
	#work 1
	yeardataDir = datadir + yeardata
	yeardatafiles = os.listdir(yeardataDir)
	reposChangesForYears(yeardataDir, yeardatafiles)

	#work 2
	godataDir = datadir + godata
	godatafiles = os.listdir(godataDir)
	#analysisGoProj(godataDir, godatafiles)

if __name__ == "__main__":
	main()
