//
// Created by Joe Pfohl on 11/25/17.
//

#ifndef C_GRAPH_HPP
#define C_GRAPH_HPP


#include <string>
#include <vector>
#include "Point.hpp"

class Graph
{
private:

    // class variables
    std::vector<std::vector<double>> matrix;
    std::vector<Point> points;

    // private internal functions
    std::vector<Point> createPoints(std::ifstream* pointFile);

public:

    Graph(std::string inputFile);

    ~Graph();

    unsigned long getWidth() const;

    std::vector<std::vector<double>> createMatrix();
};


#endif //C_GRAPH_HPP
