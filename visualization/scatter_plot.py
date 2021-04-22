import sys
sys.path.insert(1, 'Class')
import DataClass as dtc
import matplotlib.pyplot as plt


if (len(sys.argv) < 2) :
	print("Give a file")
	exit()
filename = sys.argv.pop(1)

d = dtc.DataClass(filename)


try :
	id = d.header.index('Hogwarts House')
except ValueError :
	print ("Come on, theme's harry potter, not some space opera !")
	exit()

# separate the tuples in tables of houses
gr = []
hu = []
sl = []
ra = []
for tup in d.data :
	if (tup[id] == 'Gryffindor') :
		gr.append(tup)
	if (tup[id] == 'Hufflepuff') :
		hu.append(tup)
	if (tup[id] == 'Slytherin') :
		sl.append(tup)
	if (tup[id] == 'Ravenclaw') :
		ra.append(tup)

# create separate instances
gr = dtc.DataClass(data = gr, header = d.header)
hu = dtc.DataClass(data = hu, header = d.header)
sl = dtc.DataClass(data = sl, header = d.header)
ra = dtc.DataClass(data = ra, header = d.header)

gr.init_meta()
hu.init_meta()
sl.init_meta()
ra.init_meta()

col = {'gr' : 'red', 'hu' : 'yellow', 'sl' : 'lime', 'ra' : 'blue'}
lab = {'gr' :'Gryffindor', 'hu' : 'Hufflepuff', 'sl' : 'Slytherin', 'ra' : 'Ravenclaw'}

id1 = 7
id2 = 9


dots = {'x' : [], 'y' : []}
for tup in ra.data :
	if (type(tup[id1]) == float and type(tup[id2]) == float) :
		dots['x'].append(tup[id1])
		dots['y'].append(tup[id2])
plt.plot(dots['x'], dots['y'], 'o', color=col['ra'], label=lab['ra'])

dots = {'x' : [], 'y' : []}
for tup in gr.data :
	if (type(tup[id1]) == float and type(tup[id2]) == float) :
		dots['x'].append(tup[id1])
		dots['y'].append(tup[id2])
plt.plot(dots['x'], dots['y'], 'o', color=col['gr'], label=lab['gr'])

dots = {'x' : [], 'y' : []}
for tup in hu.data :
	if (type(tup[id1]) == float and type(tup[id2]) == float) :
		dots['x'].append(tup[id1])
		dots['y'].append(tup[id2])
plt.plot(dots['x'], dots['y'], '.', color=col['hu'], label=lab['hu'])

dots = {'x' : [], 'y' : []}
for tup in sl.data :
	if (type(tup[id1]) == float and type(tup[id2]) == float) :
		dots['x'].append(tup[id1])
		dots['y'].append(tup[id2])
plt.plot(dots['x'], dots['y'], '.', color=col['sl'], label=lab['sl'])

plt.legend(prop={'size': 10})
plt.title(d.header[id1] + ' vs ' + d.header[id2])

plt.show()
