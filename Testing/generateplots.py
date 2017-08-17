#input: csv file (sample: storage in bytes_epsilon.csv)

import matplotlib.pyplot as plt
import numpy as np

filename=input()
data =  np.genfromtxt(filename, delimiter=',',names=True)
yaxis,xaxis=filename.split("_")
xaxis=xaxis.split(".")[0]
if xaxis!='skew':
	x=np.log10(data[xaxis])
else:
	x=data[xaxis]
algo=['frequent','doublefrequent','sketchfrequent']
for i in algo:
	plt.plot(x,data[i],linewidth=2)
'''
uncomment the following lines in case of overlapping lines in the graph
'''
#plt.plot(x,data['doublefrequent'],linestyle='dashed',marker='*')	
#plt.plot(x,data['sketchfrequent'],linestyle='dotted',marker='o', drawstyle='steps',markevery=2)	
if xaxis=='epsilon' or xaxis=='phi':
	plt.xlabel(xaxis+' in log scale')
else:
	plt.xlabel(xaxis)

'''
uncomment depending on the parameters being plotted
'''
#plt.ylim((-0.05,1.05))
#plt.xlim((1.1,2.1))
#plt.xlim((-4.5,-1.0))

plt.ylabel(yaxis)

#position of the legend
plt.legend(algo, loc='center right')
plt.show()
