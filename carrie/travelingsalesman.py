import math
import random
from timeit import default_timer as timer

#create a class for the cities
class City: 
	def __init__(self, id, x = None, y = None): 
		if x is None and y is None:
			self.x = random.randint(0, 200)
                	self.y = random.randint(0, 200) 
		else:
			self.x = x
			self.y = y
		self.id = id
	
	def getX(self):
		return self.x	

	def getY(self):
		return self.y

	def distanceTo(self, city):
		xDistance = self.x - city.getX()
		yDistance = self.y - city.getY()
		return math.sqrt((xDistance * xDistance) + (yDistance*yDistance))


class Tour: 
	#create blank tour
	def __init__(self, otherTour = None): 
		self.distance = 0;
                self.tourList = []
		if otherTour is None:
			self.tourList = [None] * len(TourManager) 
		else:
			self.tourList.extend(otherTour)
					
	def getTour(self):
		return self.tourList	

	def getCity(self, tourPosition):
                return self.tourList[tourPosition]

	def setCity(self, tourPosition, city):
		self.tourList[tourPosition] = city
                self.distance = 0	

	def generateIndividual(self):
		for i in range (0, len(TourManager)):
			self.setCity(i, TourManager[i])
		random.shuffle(self.tourList)	
	
	def getDistance(self):
		if self.distance == 0:
			tourDistance = 0
			for i in range(0, self.tourSize()):
				fromCity = self.getCity(i)
				if i+1 < self.tourSize():
					destinationCity = self.getCity(i+1)
				else:
					destinationCity = self.getCity(0)
			tourDistance += fromCity.distanceTo(destinationCity)
		
			self.distance = tourDistance
		return self.distance

	def tourSize(self):
		return len(self.tourList)

#get the probabilty for acceptance
def acceptance(currEnergy, newEnergy, currTemp):
	if newEnergy < currEnergy:
		return 1
	else:
		return math.exp(((currEnergy - newEnergy) / currTemp ))

def traveling_salesman_SA(TourManager):
	#set initial temperature
	temp= 10000 

	#set cooling rate
	cool = 0.003

	#create tour to start with 
	currentSolution = Tour() 
	currentSolution.generateIndividual() #add destination cities to the tour 

	#create tour to store our best solution for each iteration
	best = Tour(currentSolution.getTour())

	while (temp > 1):
		print "TEMP:" , temp
		#create another new tour as a comparison
		newSolution = Tour(currentSolution.getTour())		

		#grab random positions in the tour
		position1 = random.randint(0,newSolution.tourSize() - 1)	
		position2 = random.randint(0,newSolution.tourSize() - 1) 

		#use those positions to grab cities
		city1 = newSolution.getCity(position1)
		city2 = newSolution.getCity(position2)

		#swap the cities
		newSolution.setCity(position2, city1)
		newSolution.setCity(position1, city2)

		#get energy of solutions
		currentSolutionEnergy = currentSolution.getDistance()
		newSolutionEnergy = newSolution.getDistance()

		#decide if neighboring tour is better
		accept = acceptance(currentSolutionEnergy, newSolutionEnergy, temp)
		if accept: 
			currentSolution = Tour(newSolution.getTour())

		#keep track of current best solution found
		if currentSolution.getDistance() < best.getDistance(): 
			best = Tour(currentSolution.getTour())

		#cool the system each iteration
		temp = temp - cool
		
	return best	

TourManager = [] #array to hold cities

#open file containing activities
# open up the data.txt file
with open('tsp.txt') as file:
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
                        string = "city" #each activity will be named "activity" followed by a number
                        city_id = arrayLine[0] #assign id
			arrayLine.pop()
			city_x = arrayLine[0] #assign start time
                        arrayLine.pop()
			city_y = arrayLine[0] #assign end time       
                        arrayLine.pop()
			string += `i`
                        string = City(city_id, city_x, city_y) 
                        TourManager.append(string) #add object to list of activities
                        i += 1
	
file.close() # close file
 
#call function
start = timer()
best = traveling_salesman_SA(TourManager) 
end = timer()
runtime1 = (end - start)
print "running time:", runtime1

for i in range(0, len(TourManager)):
	print best.getCity(i).id
