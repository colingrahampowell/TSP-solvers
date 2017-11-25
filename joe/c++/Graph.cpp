//
// Created by Joe Pfohl on 11/25/17.
//

#include <fstream>
#include "Graph.hpp"

int Graph::getWidth()
{
    return width;
}

Graph::Graph(const std::string inputFile)
{
    // save value of file name
    pointFile = inputFile;

    // open the input file
    std::ifstream infile = std::ifstream();
    infile.open(inputFile);

    // generate the points vector
    points = createPoints(&infile);

    // close the input file
    infile.close();

    // create the matrix
    matrix = createMatrix();

    // create the nth references
    nthReferences = createNthReferences();
}

std::vector<Point> Graph::createPoints(std::ifstream* pointFile)
{
    // create the temporary vector
    std::vector<Point> tempPoints = std::vector<Point>();

    // create int to track width
    int tempWidth = 0;

    // read in the values and append points to the vector for each line in the file
    int id, x, y;
    while (*pointFile >> id >> x >> y)
    {
        tempPoints.emplace_back(Point(id, x, y));
        tempWidth++;
    }

    width = tempWidth;

    // return the built vector
    return tempPoints;
}

std::vector<std::vector<double>> Graph::createMatrix()
{
    // create temporary containers
    std::vector<std::vector<double>> tempGraph;

    // iterate over points adding a new vector for each point in points
    for (auto &it_a : points)
    {
        // create a temporary empty row
        std::vector<double> tempRow;

        // build the row for the point
        for (auto &point : points)
        {
            if (it_a.getId() == point.getId())
            {
                tempRow.push_back(0.0);
            }
            else
            {
                tempRow.push_back(it_a.distanceTo(point));
            }
        }

        // push the row to the back of tempGraph
        tempGraph.push_back(tempRow);
    }

    // return the built graph
    return tempGraph;
}

bool Graph::isVisited(int pointNumber)
{
    return points[pointNumber].isVisited();
}

void Graph::setVisited(int pointNumber, bool value)
{
    points[pointNumber].setVisited(value);
}

std::vector<std::vector<double>> Graph::createNthReferences()
{
    std::vector<std::vector<double>> tempNth;

    for (auto tempVect : matrix)
    {
        std::sort(tempVect.begin(), tempVect.end());
        tempNth.emplace_back(std::vector<double>(tempVect.begin() + 1, tempVect.begin() + 3));
    }

    return tempNth;
}

double Graph::getNthEdge(int pointNum, int n)
{
    if (n > 1 || pointNum > getWidth())
    {
        return -1;
    }
    else
    {
        return nthReferences[pointNum][n];
    }
}

double Graph::distance(int p1, int p2)
{
    if (p1 > getWidth() || p2 > getWidth())
    {
        return -1;
    }
    else
    {
        return matrix[points[p1].getId()][points[p2].getId()];
    }
}

std::string Graph::getPointFileName()
{
    return pointFile;
}

std::vector<std::vector<double>>::iterator Graph::matrixBegin()
{
    return matrix.begin();
}

std::vector<std::vector<double>>::iterator Graph::matrixEnd()
{
    return matrix.end();
}

std::vector<Point>::iterator Graph::pointsBegin()
{
    return points.begin();
}

std::vector<Point>::iterator Graph::pointsEnd()
{
    return points.end();
}

int Graph::getPointId(int n)
{
    return points[n].getId();
}

