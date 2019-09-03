#original data in a file called op.in
#Output of Frequent/DoubleFrequent/SketchFrequent given as input to this program

#to be varied
phi=0.01

m=int(input())
d=dict()
for i in range(m):
	r = input()
	if r not in d.keys() :
		d[r]=1
	else:
		d[r]+=1
thresh=phi*m
t={i:d[i] for i in d if d[i]>=phi*m}

truereported=0
totalreported=0
avg_re_true=0.0
avg_re_false=0.0
f=open("op.in","r")
time=f.readline()
size=f.readline()
l=f.readline()
while l is not "":
	item,count=l.split(":")
	l=f.readline()
	if item in t:
		truereported+=1
		avg_re_true+=float(abs(t[item]-int(count[:len(count)]))/t[item])
	else:
		avg_re_false+=float(abs(d[item]-int(count[:len(count)]))/d[item])
	totalreported+=1
print("time: ",time)
print("size: ",size)
print("phi: ",phi)
print("m: ",m)
if len(t)>0:
	print("recall: ",truereported/len(t))
else:
	print("recall: 1")
if totalreported>0:
	print("precision: ",truereported/totalreported)
else:
	print("precision: 1")
if truereported>0:
	print("avg relative error(true heavy hitters): ",avg_re_true/truereported)	
falsepositives=totalreported-truereported
if falsepositives>0:
	print("avg relative error(false positives): ",avg_re_false/falsepositives)