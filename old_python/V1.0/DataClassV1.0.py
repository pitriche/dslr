import MetaData as md
import math

class DataClass :
	'v1.0 of DataClass, a data storing and parsing class'
	
	def __init__(self, file_stream) :
		self.file_stream = file_stream
		self.header = ""
		self.col = 0
		self.row = 0
		self.data = []
		self.meta = md.MetaData()


	def __parse_header(self) :
		self.header = self.file_stream.readline()
		sp = self.header.split(',')
		#stripping to remove newline
		for i in range(len(sp)) :
			sp[i] = sp[i].strip()
		self.header = tuple(sp)
		self.col = len(self.header)
		self.data_sorted = []
		for i in range(self.col) :
			self.data_sorted.append([])


	def __parse_tuple(self, sp) :
		i = 0
		for d in sp :
			if (d.isdigit()) :
				sp[i] = int(d)
			else :
				try :
					sp[i] = float(d)
				except ValueError :
					pass
			i += 1
		return (tuple(sp))


	def __read_data(self) :
		line = self.file_stream.readline()
		while line :
			sp = line.split(',')

			# for each lines, split contain all data of the tuple
			for i in range(self.col) :
				
				# assign the data to feature lists if numerical
				if (not sp[i].isdigit() and sp[i] != "Nan") :
					try :
						self.data_sorted[i].append(float(sp[i]))
						if (i == 2) :
							print (sp[i])
					except ValueError :
						pass

			#apprend data as parsed tuple in main list
			self.data.append(self.__parse_tuple(sp))
			line = self.file_stream.readline()
		self.row = len(self.data)


	def __parse_metadata(self) :
		self.meta.init(self.col)
		for i in range(self.col) :
			# count
			self.meta.count[i] = float(len(self.data_sorted[i]))

			# sort
			self.data_sorted[i].sort()

			# quartiles
			self.meta.min[i] = md.quartile(self.data_sorted[i], 0)
			self.meta.q25[i] = md.quartile(self.data_sorted[i], 0.25)
			self.meta.q50[i] = md.quartile(self.data_sorted[i], 0.5)
			self.meta.q75[i] = md.quartile(self.data_sorted[i], 0.75)
			self.meta.max[i] = md.quartile(self.data_sorted[i], 1)

			# mean calculation
			for j in self.data_sorted[i] :
				self.meta.mean[i] += j
			if (len(self.data_sorted[i])) :
				self.meta.mean[i] /= len(self.data_sorted[i])

			# standard deviation calculation
			for j in self.data_sorted[i] :
				self.meta.std[i] += (self.meta.mean[i] - j) ** 2
			if (len(self.data_sorted[i])) :
				self.meta.std[i] /= len(self.data_sorted[i])
			self.meta.std[i] = math.sqrt(self.meta.std[i])


	def init(self) :
		self.__parse_header()
		#print("reading data ...")
		self.__read_data()
		#print("parsing data ...")
		self.__parse_metadata()
		#print("Complete !")


	def __disp_header(self) :
		print("      ", end='|')
		for i in self.header :
			print("%11.11s" % i, end = '|')
		print()


	def disp(self) :
		#print(f"data has {self.row} tuples with {self.col} features in each")
		#print("Metadata:")
		self.__disp_header()
		self.meta.disp()
