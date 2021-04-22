import sys
import DataClass as dtc


if (len(sys.argv) < 2) :
	print("Give a file")
	exit()
filename = sys.argv.pop(1)
try :
	data = open(filename, "r")
except IOError :
	print("Wrong file duh.")
	exit()

d = dtc.DataClass(data)
d.init()
d.disp()