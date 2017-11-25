//
// Created by Joe Pfohl on 11/25/17.
//

#include <fstream>
#include "Graph.hpp"

unsigned long Graph::getWidth() const
{
    return points.size();
}

Graph::~Graph()
{

}


Graph::Graph(const std::string inputFile)
{
    // open the input file
    std::ifstream infile = std::ifstream();
    infile.open(inputFile);

    // generate the points vector
    points = createPoints(&infile);

    // create the matrix
    matrix = createMatrix();
}

std::vector<Point> Graph::createPoints(std::ifstream* pointFile)
{
    // create the temporary vector
    std::vector<Point> tempPoints = std::vector<Point>();

    // read in the values and append points to the vector for each line in the file
    int id, x, y;
    while (*pointFile >> id >> x >> y)
    {
        tempPoints.emplace_back(Point(id, x, y));
    }

    // return the built vector
    return tempPoints;
}

std::vector<std::vector<double>> Graph::createMatrix()
{
    // create temporary containers
    std::vector<std::vector<double>> tempGraph;

    // iterate over points adding a new vector for each point in points
    for (auto it_a = std::begin(points); it_a != std::end(points); ++it_a)
    {
        // create a temporary empty row
        std::vector<double> tempRow;

        // build the row for the point
        for (auto it_b = std::begin(points); it_b != std::end(points); ++it_b)
        {
            if (it_a == it_b)
            {
                tempRow.push_back(0.0);
            }
            else
            {
                tempRow.push_back(it_a->distanceTo(*it_b));
            }
        }

        // push the row to the back of tempGraph
        tempGraph.push_back(tempRow);
    }

    // return the built graph
    return tempGraph;
}

