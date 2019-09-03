#input: csv file (sample: storage in bytes_epsilon.csv filename: yaxisparameter_xaxisparameter.csv)
#format of csv file: 4 columns: (xaxisparameter, frequent, doublefrequent, sketchfrequent)

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
algo=['Frequent','DoubleFrequent','SketchFrequent','DoubleLC','SketchLC','LCDelta']
linestyles=['--','-','-.',':','-','-.']
markers=['.','^','2','p','*','D']
colors=['b','g','r','c','k','y']
index=0
for i in algo:
	plt.plot(x,data[i],color=colors[index],linewidth=2,marker=markers[index],linestyle=linestyles[index],markerfacecolor=colors[index])
	index+=1
	
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
#for skew
plt.xlim((1.0,2.2))
plt.ylabel(yaxis)

#position of the legend
plt.legend(algo, loc='center right')
plt.show()