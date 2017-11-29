#################################################
# File: makefile
# Description: makefile for CS325, final proejct 
# Note: Based on "Robust Makefile" provided with
# Oregon State University CS162 course material.
#################################################

# set compiler, flags
CXX = g++
CXXFLAGS = -Wall -O3 -fopenmp


#################################################
# USER SECTION 
#################################################

# source files
SRC1 = 2opt-pp.c
SRC2 = point.c
SRC3 = ptList.c
SRCS = ${SRC1} ${SRC2} ${SRC3}

# executable files
PROG = tsp-2opt 

#################################################
# BUILD SECTION 
#################################################

# compile tsp-2opt 
tsp:
	${CXX} ${CXXFLAGS} ${SRCS} -o ${PROG}

# compile with -g flag for valgrind debugging
debug:
	${CXX} ${CXXFLAGS} -g ${SRCS} -o ${PROG}

# remove all executable files and debug files
clean:
	rm -f -v *~ ${PROG} *.dSYM
