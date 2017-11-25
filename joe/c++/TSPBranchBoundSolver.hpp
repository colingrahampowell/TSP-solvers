//
// Created by Joe Pfohl on 11/25/17.
//

#ifndef C_TSPBRANCHBOUNDSOLVER_HPP
#define C_TSPBRANCHBOUNDSOLVER_HPP


#include "Graph.hpp"

class TSPBranchBoundSolver
{
private:

    // variables
    Graph graph;
    std::vector<int> path;
    std::vector<int> finalPath;
    int level;
    double bound;
    double pathWeight;

    // methods
    void setStartingBound();
    void autoUpdateBound();
    void setVisitedAtCurrentLevel(bool value);
    void recursiveTSPHelper(double weight);
    void setPathWeight(double weight);
    void setBound(double bound);
    double getBound();
    void setLevel(int level);
    int getLevel();
    void addPointToPath(int level, int pointNum);

public:

    // constructor & getter/setters
    explicit TSPBranchBoundSolver(Graph graph);
    double getPathWeight();

    // methods
    void findPath();
    void print_results(bool toFile=false);
};


#endif //C_TSPBRANCHBOUNDSOLVER_HPP
