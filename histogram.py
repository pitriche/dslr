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

col = ['red', 'yellow', 'lime', 'blue']
lab = ['Gryffindor', 'Hufflepuff', 'Slytherin', 'Ravenclaw']

view = "all"
#view = "dual"

if (view == "dual") :
	fig, plots = plt.subplots(nrows=1, ncols=2)
	id = 6
	aryth = [[], [], [], []]
	aryth[0] = gr.sorted[id] * 16
	aryth[1] = hu.sorted[id] * 10
	aryth[2] = sl.sorted[id] * 18
	aryth[3] = ra.sorted[id] * 12
	plots[0].hist(aryth, color=col, label=lab)
	plots[0].legend(prop={'size': 10})
	plots[0].set_title(d.header[id] + ' mark repartition')
	id = 16
	care = [[], [], [], []]
	care[0] = gr.sorted[id] * 16
	care[1] = hu.sorted[id] * 10
	care[2] = sl.sorted[id] * 18
	care[3] = ra.sorted[id] * 12
	plots[1].hist(care, color=col, label=lab)
	plots[1].legend(prop={'size': 10})
	plots[1].set_title(d.header[id] + ' mark repartition')
elif (view == "all") :
	fig, plot = plt.subplots(nrows=4, ncols=4)
	plots = plot.flatten()
	id = 6
	while (id < 19) :
		grade = [[], [], [], []]
		grade[0] = gr.sorted[id] * 16
		grade[1] = hu.sorted[id] * 10
		grade[2] = sl.sorted[id] * 18
		grade[3] = ra.sorted[id] * 12
		plots[id - 6].hist(grade, color=col, label=lab)
		if (id == 6) :
			plots[id - 6].legend(prop={'size': 7})
		plots[id - 6].set_title(d.header[id] + ' mark repartition')
		id += 1
	print("Gryffindor")
	gr.describe()
	print("\nHufflepuff")
	hu.describe()
	print("\nSlytherin")
	sl.describe()
	print("\nRavenclaw")
	ra.describe()

plt.show()
