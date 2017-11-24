import sys

from graph import Graph
from path


def main():
    """ main function """

    # validate command line parrams
    if len(sys.argv) is not 2:
        print "Usage: python tsp_bb.py <input_file.txt>"
        exit(1)

    graph = Graph(sys.argv[1])
    path

    print "test"


if __name__ == "__main__":
    main()
