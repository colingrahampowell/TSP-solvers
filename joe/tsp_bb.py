import sys
from point import Point


def main():
    """ main function """

    # validate command line parrams
    if len(sys.argv) is not 2:
        print "Usage: python tsp_bb.py <input_file.txt>"
        exit(1)

    # get name of file to read from
    file = sys.argv[1]

    # open and read in all arguments
    read_buf = []
    with open(file, 'r') as r:
        for line in r:
            line = line.rstrip()
            if not line: continue
            read_buf.append(line)

    # convert array of strings to an array of points
    points = [Point(x[0], x[1], x[2]) for x in [[int(y) for y in line.split(' ')] for line in read_buf]]


if __name__ == "__main__":
    main()
