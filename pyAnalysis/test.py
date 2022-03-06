import pandas as pd
import calmap
import matplotlib.pyplot as plt

def processDate(datestr):
	if datestr:
		dt = datestr.split('T')[0]
		return dt
	return None

fd = open("result/commit.dat", "r")
lines = fd.readlines()
fd.close()

df = pd.DataFrame(lines)
print(df)

se = df[0].apply(processDate).value_counts().rename_axis("date").reset_index(name='counts')
se['date'] = pd.to_datetime(se['date'])
se.set_index("date", inplace=True)
print(se)
se.info()
se.plot()
print(se['counts'])
calmap.calendarplot(se['counts'], monthticks=False, dayticks=False, fig_kws=dict(figsize=(256,64)))
plt.show()
