import math
import random

# City class:

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


# Tour class: 

class Tour: 

    #create blank tour - cityList is a list of cities
    def __init__(self, cityList, otherTour = None): 

        self.distance = 0
        self.tourList = []

        if otherTour is None:
            self.tourList = [None] * len(cityList) 

        else:
            self.tourList.extend(otherTour)
                
    # returns an entire tour    
    def getTour(self):
        return self.tourList    

    # gets city at defined position in tour
    def getCity(self, tourPosition):
        return self.tourList[tourPosition]

    # sets city at defined position in tour
    def setCity(self, tourPosition, city):
        self.tourList[tourPosition] = city
        self.distance = 0    

    # generate a random tour - cityList is a list of cities
    def generateIndividual(self, cityList):
        for i in range (0, len(cityList)):
            self.setCity(i, cityList[i])
            random.shuffle(self.tourList)    

    # gets total tour distance
    def getDistance(self):
        if self.distance == 0:
            tourDistance = 0

        for i in range(0, self.tourSize()):
            fromCity = self.getCity(i)

            if i + 1 < self.tourSize():
                destinationCity = self.getCity(i+1)
            else:
                destinationCity = self.getCity(0)

        tourDistance += fromCity.distanceTo(destinationCity)
        self.distance = tourDistance

        return self.distance

    # gets number of cities in tour 
    def tourSize(self):
        return len(self.tourList)
