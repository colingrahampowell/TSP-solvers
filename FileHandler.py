import City.py

# read a TSP file, process contents
def readFile(filename):

	cityList = []

	# open up the data.txt file
	with open(filename) as file:

		#read each value as an integer into the array
		array = []
		arrayLine = []

		#create a new array to hold each object
		arr_objects = []

		for line in file:

			values = line.split() # this splits up the number by spaces
			array = [int(x) for x in values] # this casts the value as integers

			print line
			print array

			for element in line.split(' '):
				arrayLine.append(int(element))
				print element


			#create objects for each activity
			x = 0

			while arrayLine:

				i = 1
		#		string = "city" #each city will be named "city" followed by a number

				city_id = arrayLine[0] #assign id
				arrayLine.pop()

				city_x = arrayLine[0] #assign start time
				arrayLine.pop()

				city_y = arrayLine[0] #assign end time       
				arrayLine.pop()

		#		string += `i`
		#		string = City(city_id, city_x, city_y) 
				cityList.append( City(city_id, city_x, city_y) ) #add object to list of cities 

				i += 1

	file.close() # close file
	return cityList


# run an instance of our TSP test
def run_test( algorithm, cityList ):

	#call function
	start = timer()
	best = algorithm( cityList ) 
	end = timer()

	runtime1 = (end - start)
	print "running time: ", runtime1

	for i in range(0, len(cityList)):
		print best.getCity(i).id


