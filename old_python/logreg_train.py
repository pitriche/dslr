import sys
sys.path.insert(1, 'Class')
import DataClass as dtc
import math

#import matplotlib.pyplot as plt


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

def single_cost (x, y, theta) :
	ht = h_theta(x, theta)
	if (ht == 0.0 or ht == 1.0) :
		if (ht != y) :
			return (1e100)
		else :
			return (0.0)
	return (-(y * math.log(ht) + (1 - y) * math.log(1 - ht)))

def global_cost (x2, y2, theta) :
	res = 0
	for i in range(len(x2)) :
		res += single_cost(x2[i], y2[i], theta)
	return (res / len(x2))

def train_cycle(x, y, theta, lr) :
	deriv = [0] * (len(x[0]) + 1)
	for i in range(len(x)) :
		ht = h_theta(x[i], theta)
		yi = y[i]
		deriv[0] += ht - yi

		for j in range(len(x[i])) :
			deriv[j + 1] += (ht - yi) * x[i][j]
	
	#print("derivatives: " + str(deriv))
	for i in range(len(deriv)) :
		theta[i] -= deriv[i] * lr

# ############################################################################ #
# ############################   main implement   ############################ #
# ############################################################################ #

if (len(sys.argv) < 2) :
	print("Give a file")
	exit()
filename = sys.argv.pop(1)
d = dtc.DataClass(filename)

ids = [7, 8, 10, 11, 12, 13, 14, 17, 18]
#ids = [7, 8, 10, 11, 12, 13, 14, 15, 17, 18]
idhh = d.header.index("Hogwarts House")
theta = {'gr': 0, 'hu': 0, 'sl': 0, 'ra': 0}
theta['gr'] = [0] * (len(ids) + 1)
theta['hu'] = [0] * (len(ids) + 1)
theta['sl'] = [0] * (len(ids) + 1)
theta['ra'] = [0] * (len(ids) + 1)

x = []
y = {'gr': [], 'hu': [], 'sl': [], 'ra': []}

def houses(tup, y) :
	y['gr'].append(0)
	y['hu'].append(0)
	y['sl'].append(0)
	y['ra'].append(0)

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
		houses(tup, y)

iter = 500
for i in range(iter) :
	if (i % (iter / 10) == 0 or i + 1 == iter) :
		print("Training [" + str(i) + "/" + str(iter) + "]")
		print("gr cost :", round(global_cost(x, y['gr'], theta['gr']), 4))
		print("hu cost :", round(global_cost(x, y['hu'], theta['hu']), 4))
		print("sl cost :", round(global_cost(x, y['sl'], theta['sl']), 4))
		print("ra cost :", round(global_cost(x, y['ra'], theta['ra']), 4))
		print()
	train_cycle(x, y['gr'], theta['gr'], 0.00000005)
	train_cycle(x, y['hu'], theta['hu'], 0.000000025)
	train_cycle(x, y['sl'], theta['sl'], 0.00000006)
	train_cycle(x, y['ra'], theta['ra'], 0.00000006)

print("Training [" + str(iter) + "/" + str(iter) + "] Complete !")
print("cost:", global_cost(x, y['gr'], theta['gr']))

file_the = open("thetafile", "w")
for i in range(len(theta['gr'])) :
	file_the.write(str(theta['gr'][i]) + '\n')
	file_the.write(str(theta['hu'][i]) + '\n')
	file_the.write(str(theta['sl'][i]) + '\n')
	file_the.write(str(theta['ra'][i]) + '\n\n')