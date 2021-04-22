import sys
sys.path.insert(1, 'Class')
import DataClass as dtc
import math

def sig(z) :
	try :
		return (1 / (1 + math.e ** -z))
	except OverflowError :
		return (0.0)

def h_theta(x, theta) :
	tot = theta[0]
	for i in range(len(x)) :
		tot += theta[i + 1] * x[i]
	return (sig(tot))

# ############################################################################ #

if (len(sys.argv) < 2) :
	print("Give a file")
	exit()
filename = sys.argv.pop(1)
d = dtc.DataClass(filename)

# ############################################################################ #
testacc = False
# ############################################################################ #


ids = [7, 8, 10, 11, 12, 13, 14, 17, 18]
# ids = [7, 8, 10, 11, 12, 13, 14, 15, 17, 18]
idx = []
idhh = d.header.index("Hogwarts House")
theta = {'gr': 0, 'hu': 0, 'sl': 0, 'ra': 0}
theta['gr'] = [0] * (len(ids) + 1)
theta['hu'] = [0] * (len(ids) + 1)
theta['sl'] = [0] * (len(ids) + 1)
theta['ra'] = [0] * (len(ids) + 1)

x = []
y = {'gr': [], 'hu': [], 'sl': [], 'ra': []}
yh = []

def houses(tup, y, yh) :
	y['gr'].append(0)
	y['hu'].append(0)
	y['sl'].append(0)
	y['ra'].append(0)
	yh.append(tup[idhh])

	if (tup[idhh] == "Gryffindor") :
		y['gr'][-1] = 1
	elif (tup[idhh] == "Hufflepuff") :
		y['hu'][-1] = 1
	elif (tup[idhh] == "Slytherin") :
		y['sl'][-1] = 1
	elif (tup[idhh] == "Ravenclaw") :
		y['ra'][-1] = 1

for tup in d.data :
	ok = True
	tmp = []
	for i in ids :
		if (type(tup[i]) != float) :
			ok = False
		tmp.append(tup[i])
	if (ok) :
		x.append(tmp)
		idx.append(tup[0])
		if (testacc) :
			houses(tup, y, yh)




file_the = open("thetafile", "r")
for i in range(len(theta['gr'])) :
	theta['gr'][i] = float(file_the.readline())
	theta['hu'][i] = float(file_the.readline())
	theta['sl'][i] = float(file_the.readline())
	theta['ra'][i] = float(file_the.readline())
	file_the.readline()
#print("THETA=", theta)

def house(predict) :
	g = predict['gr']
	h = predict['hu']
	s = predict['sl']
	r = predict['ra']
	if (g >= h and g >= s and g >= r) :
		return ("Gryffindor")
	elif (h >= g and h >= s and h >= r) :
		return ("Hufflepuff")
	elif (s >= g and s >= h and s >= r) :
		return ("Slytherin")
	elif (r >= g and r >= h and r >= s) :
		return ("Ravenclaw")
	else :
		print("mdr le dataset il trolle")
		exit()


if (testacc) :
	acc = 0
	for i in range(len(x)) :
		predict = {'gr': 0, 'hu': 0, 'sl': 0, 'ra': 0}
		predict['gr'] = h_theta(x[i], theta['gr'])
		predict['hu'] = h_theta(x[i], theta['hu'])
		predict['sl'] = h_theta(x[i], theta['sl'])
		predict['ra'] = h_theta(x[i], theta['ra'])
		if (house(predict) == yh[i]) :
			acc += 1
	print("acc score:", round(acc * 100.0 / len(yh), 2), "%")
else:
	file_house = open("houses.csv", "w")
	for i in range(len(x)) :
		predict = {'gr': 0, 'hu': 0, 'sl': 0, 'ra': 0}
		predict['gr'] = h_theta(x[i], theta['gr'])
		predict['hu'] = h_theta(x[i], theta['hu'])
		predict['sl'] = h_theta(x[i], theta['sl'])
		predict['ra'] = h_theta(x[i], theta['ra'])
		file_house.write(str(idx[i]) + "," + house(predict) + '\n')