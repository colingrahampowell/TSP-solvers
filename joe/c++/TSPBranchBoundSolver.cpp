//
// Created by Joe Pfohl on 11/25/17.
//

#include <iostream>
#include <fstream>
#include "TSPBranchBoundSolver.hpp"

void TSPBranchBoundSolver::setStartingBound()
{
    setBound(0);
    for (auto p_itr = graph.pointsBegin(); p_itr != graph.pointsEnd(); p_itr++)
    {
        setBound(getBound() + ((graph.getNthEdge(p_itr->getId(), 0) + graph.getNthEdge(p_itr->getId(), 1)) / 2));
    }
}

void TSPBranchBoundSolver::autoUpdateBound()
{
    if (getLevel() == 0)
    {
        setStartingBound();
    }
    else if (getLevel() == 1)
    {
        setBound(getBound() - ((graph.getNthEdge(0, 0) + graph.getNthEdge(1, 0)) / 2));
    }
    else
    {
        setBound(getBound() - ((graph.getNthEdge(getLevel() - 1, 1) + graph.getNthEdge(getLevel(), 0)) / 2));
    }
}

void TSPBranchBoundSolver::setVisitedAtCurrentLevel(bool value)
{
    graph.setVisited(path[getLevel()], value);
}

void TSPBranchBoundSolver::recursiveTSPHelper(double weight)
{
    if (getLevel() == graph.getWidth())
    {
        double finalWeight = weight + graph.distance(path[getLevel() - 1], path[0]);
        if (finalWeight < getPathWeight())
        {
            std::cout << "New path found: " << finalWeight << std::endl;
            setPathWeight(finalWeight);
            finalPath = path;
        }
    }
    else
    {
        for (auto p_itr = graph.pointsBegin(); p_itr != graph.pointsEnd(); ++p_itr)
        {
            if (!p_itr->isVisited())
            {
                double newWeight = weight + graph.distance(path[getLevel() - 1], p_itr->getId());
                double saveBound = getBound();
                autoUpdateBound();
                if ((newWeight + getBound()) < getPathWeight())
                {
                    addPointToPath(getLevel(), p_itr->getId());
                    setVisitedAtCurrentLevel(true);
                    setLevel(getLevel() + 1);
                    recursiveTSPHelper(newWeight);
                    setLevel(getLevel() - 1);
                    setVisitedAtCurrentLevel(false);
                    addPointToPath(getLevel(), -1);
                }
                setBound(saveBound);
            }
        }
    }
}

int TSPBranchBoundSolver::getLevel()
{
    return level;
}

void TSPBranchBoundSolver::setLevel(int level)
{
    this->level = level;
}

double TSPBranchBoundSolver::getPathWeight()
{
    return pathWeight;
}

void TSPBranchBoundSolver::setPathWeight(double weight)
{
    pathWeight = weight;
}

void TSPBranchBoundSolver::findPath()
{
    addPointToPath(getLevel(), graph.getPointId(0));
    setVisitedAtCurrentLevel(true);
    setLevel(getLevel() + 1);
    recursiveTSPHelper(0);
}

void TSPBranchBoundSolver::print_results(bool toFile)
{
    if (toFile)
    {
        std::string outFileName = graph.getPointFileName() + ".tour";
        std::ofstream outFile(outFileName, std::ofstream::out);

        outFile << getPathWeight() << std::endl;

        for (int &itr : finalPath)
        {
            outFile << itr << std::endl;
        }

        outFile.close();
    }
    else
    {
        std::cout << "Path Weight: " << getPathWeight() << std::endl;
        for (int &itr : finalPath)
        {
            std::cout << itr << std::endl;
        }
    }
}

TSPBranchBoundSolver::TSPBranchBoundSolver(Graph graph) : graph(graph)
{
    // create path filled with -1
    path = std::vector<int>(graph.getWidth(), -1);

    // set starting level and path weight
    setLevel(0);
    setPathWeight(std::numeric_limits<double>::max());

    // set the starting bound
    setStartingBound();
}

double TSPBranchBoundSolver::getBound()
{
    return bound;
}

void TSPBranchBoundSolver::setBound(double bound)
{
    this->bound = bound;
}

void TSPBranchBoundSolver::addPointToPath(int level, int pointNum)
{
    path[level] = pointNum;
}
