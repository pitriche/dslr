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

fig, plots = plt.subplots(nrows=10, ncols=10)
spl = plots.flatten()

def scatter(spl, id1 = 7, id2 = 9) :
	dots = {'x' : [], 'y' : []}
	for tup in ra.data :
		if (type(tup[id1]) == float and type(tup[id2]) == float) :
			dots['x'].append(tup[id1])
			dots['y'].append(tup[id2])
	spl.plot(dots['x'], dots['y'], 'o', color=col['ra'])

	dots = {'x' : [], 'y' : []}
	for tup in gr.data :
		if (type(tup[id1]) == float and type(tup[id2]) == float) :
			dots['x'].append(tup[id1])
			dots['y'].append(tup[id2])
	spl.plot(dots['x'], dots['y'], 'o', color=col['gr'])

	dots = {'x' : [], 'y' : []}
	for tup in hu.data :
		if (type(tup[id1]) == float and type(tup[id2]) == float) :
			dots['x'].append(tup[id1])
			dots['y'].append(tup[id2])
	spl.plot(dots['x'], dots['y'], '.', color=col['hu'])

	dots = {'x' : [], 'y' : []}
	for tup in sl.data :
		if (type(tup[id1]) == float and type(tup[id2]) == float) :
			dots['x'].append(tup[id1])
			dots['y'].append(tup[id2])
	spl.plot(dots['x'], dots['y'], '.', color=col['sl'])
	#spl.set_title(d.header[id1] + ' vs ' + d.header[id2])

def histo(spl, id = 7) :
	mark = [[], [], [], []]
	mark[0] = gr.sorted[id] * 16
	mark[1] = hu.sorted[id] * 10
	mark[2] = sl.sorted[id] * 18
	mark[3] = ra.sorted[id] * 12
	spl.hist(mark, color=['red', 'yellow', 'lime', 'blue'])
	#spl.set_title(d.header[id] + ' mark repartition')

ids = (7, 8, 10, 11, 12, 13, 14, 15, 17, 18)

for i in  range(len(spl)) :
	id1 = ids[int(i / 10)]
	id2 = ids[int(i % 10)]
	if (id1 != id2) :
		if (id1 < id2) :
			scatter(spl[i], id1, id2)
	else :
		histo(spl[i], id1)

#plt.legend(prop={'size': 10})
plt.show()
