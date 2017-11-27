#
# Group 34: Nearest-Neighbor + 2-Opt 
#

import Tour 
import TestHandler
import sys
import random

def nearestNeighbor( cityList ):

	new_tour = Tour.Tour(cityList)	# path of cities from cityList
	visited = []	# indices of visited cities in cityList

	# set random starting point	
	current = cityList[ random.randint( 0, len(cityList) - 1 ) ]

	# append it to the path, update path length, mark as visited
	new_tour.setCity( 0, current )	
	length = 1
	visited.append( cityList.index( current ) )

	while( length < len(cityList) ):
			
		i = 0
	
		# find first unvisited city, mark as 'min distance'
		while(i in visited):
			i = i + 1

		closest = cityList[i]

		for j in range(i, len(cityList) ):
			# if city is unvisited and closer to current city
			if j not in visited:

				if current.distanceTo( cityList[j] ) < current.distanceTo( closest ):
					# set as minDist	
					closest = cityList[j]	
	
		# upkeep: mark as visited, append to path, update v	
		visited.append( cityList.index( closest ) )
		new_tour.setCity( length, closest )

		print current.distanceTo(closest)

		current = closest	

		length += 1

	return new_tour 

"""
def twoOptSwap( ):





def twoOpt( ):
"""

if __name__ == "__main__":
	
	print "hi there"	
	
	cities = TestHandler.readFile( sys.argv[1] )	

	for i in cities:
		print cities[i] 
	

	tour = nearestNeighbor( cities )
	
	print tour.getDistance()
	
			

"""
def make_tour( ):

	# nearestNeighbor( )

	# twoOpt( ) - which should be implemented with a time limit / unlimited
"""

