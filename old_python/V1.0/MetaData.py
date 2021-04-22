import math

class MetaData :
	'v1.1 of MetaData, a class to describe a single feature'
	'included in DataClass as a dict'

	# constructor, takes data and calculate the metadata, or init to 0
	# values contain all the metadata in a dict for easier display
	def __init__(self, data = 0):
		self.values = {
			"count" : 0,
			"mean" : 0,
			"std" : 0,
			"min" : 0,
			"q25" : 0,
			"q50" : 0,
			"q75" : 0,
			"max" : 0}

		if (data == 0) :
			return
		self.count = len(data)
		if (self.count == 0) :
			return
		print (self.values)
		for d in data :
			self.mean += d
		self.mean /= self.count
		for d in data :
			self.std += (d - self.mean) ** 2
		self.std = math.sqrt(self.std / self.count)
		self.min = self.quartile(data, 0)
		self.q25 = self.quartile(data, 0.25)
		self.q50 = self.quartile(data, 0.50)
		self.q75 = self.quartile(data, 0.75)
		self.max = self.quartile(data, 1)
		self.values = {"count" : self.count, "mean" : self.mean,
			"std" : self.std, "min" : self.min,
			"q25" : self.q25, "q50" : self.q50,
			"q75" : self.q75, "max" : self.max}
		print (self.values)


	# quartile function
	def quartile(self, lst, quartile) :
		if (len(lst) == 0) :
			return (0.0)
		if(quartile > 1) :
			quartile = 1
		if(quartile < 0) :
			quartile = 0
		i = int(len(lst) * quartile)
		if (i >= len(lst)) :
			i = len(lst) - 1
		return (lst[i])
