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

def logValue(val):
	if (val <= 0):
		return 0
	return math.log(val, 1.2) 

def timestr2timestamp(timestr):
	'''
	This function converys timestring to UNIX timestamp
	'''
	if pd.isnull(timestr):
		return 0
	dt = datetime.strptime(str(timestr), '%Y-%m-%dT%H:%M:%SZ')
	return dt.timestamp()
	

def readFromFile(filename):
	fd = open(filename, "r")
	lines = fd.readlines()
	fd.close()

	return lines

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
		text = open(filename).read()
		wordcloud = wcd(background_color="white").generate(text)
		plt.imshow(wordcloud, interpolation='bilinear')
		plt.axis("off")
		plt.savefig(outputdir + fl + ".png", dpi=800)
		plt.show()

	ct = dfall["created_at"].apply(timestr2timestamp)
	size = dfall["size"].apply(logValue)
	result = pd.concat([ct, size], axis=1)
	result.plot.scatter(x="created_at", y="size", color="DarkGreen")
	plt.savefig(outputdir + "scatter.png", dpi=800)
	plt.show()


def analysisTimeline(path, files):
	'''
	this function analysis the data of number of Go project every year form 2009 to	2021
	and generate a bar char to vasulize the result
	'''
	year = []
	number = []
	files.sort()

	for fl in files:
		print("read files : %s" %fl)
		filecontent = readFromFile(path + fl)
		total = 0

		# count the total number of Go project
		for content in filecontent:
			#each line represent one month in a year
			df = pd.DataFrame(json.loads(content))
			try:
				cnt = df['language'].value_counts().loc['Go']
			except KeyError:
				cnt = 0
			total += cnt
			print("%s : %d" %(fl, total))

		year.append((int)(fl.split('.')[0]))
		number.append(total)

	plt.figure(figsize=(12, 8), dpi=80)
	plt.bar(year, number, tick_label=year, label="repos")
	plt.xlabel("year")
	plt.ylabel("Go repos per thousand")
	plt.title("Golang trends")
	plt.savefig(outputdir + "year_trends.png")
	plt.show()

def main():
	#work 1
	yeardataDir = datadir + yeardata
	yeardatafiles = os.listdir(yeardataDir)
	analysisTimeline(yeardataDir, yeardatafiles)

	#work 2
	godataDir = datadir + godata
	godatafiles = os.listdir(godataDir)
	analysisGoProj(godataDir, godatafiles)

if __name__ == "__main__":
	#clean 
	os.system("rm -rf result & rm -rf data")
	
	#copy data from build dir
	os.system("cp -r ../build/data ./")
	#mkdir result
	os.system("mkdir -p result")
	main()
