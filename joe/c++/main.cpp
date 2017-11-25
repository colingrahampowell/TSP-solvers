#include <iostream>
#include "Point.hpp"
#include "Graph.hpp"
#include "TSPBranchBoundSolver.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: tsp_bb filename.txt" <<std::endl;
        exit(1);
    }
    else
    {
        std::string fileName = std::string(argv[1]);
        Graph g1(fileName);
        TSPBranchBoundSolver solver(g1);
        solver.findPath();
        solver.print_results(false);
    }

    exit(0);
}
