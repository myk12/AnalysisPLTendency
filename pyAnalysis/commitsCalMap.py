import pandas as pd
import calmap
import matplotlib.pyplot as plt

outputdir = "result/"

def processDate(datestr):
	if datestr:
		dt = datestr.split('T')[0]
		if dt >= "2017-01-01":
			return dt
	return None

def main():
	fd = open("data/commits/commits.dat", "r")
	lines = fd.readlines()
	fd.close()

	df = pd.DataFrame(lines)

	se = df[0].apply(processDate).value_counts().rename_axis("date").reset_index(name='counts')
	se['date'] = pd.to_datetime(se['date'])
	se.set_index("date", inplace=True)
	print(se)
	se.info()
	se.plot()
	print(se['counts'])
	calmap.calendarplot(se['counts'], monthticks=False, dayticks=False, fig_kws=dict(figsize=(256,64)))
	plt.savefig(outputdir + "commits.png", bbox_inches='tight')
	plt.show()

if __name__ == "__main__":
	main()
