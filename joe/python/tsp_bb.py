import sys

from graph import Graph
from path import Path


def main():
    """ main function """

    # validate command line parrams
    if len(sys.argv) is not 2:
        print "Usage: python tsp_bb.py <input_file.txt>"
        exit(1)

    graph = Graph(sys.argv[1])
    path = Path(graph)
    path.branch_and_bound_tsp()


if __name__ == "__main__":
    main()
