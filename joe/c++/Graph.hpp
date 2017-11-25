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
    std::vector<std::vector<double>> nthReferences;
    std::vector<Point> points;
    std::string pointFile;

    // private internal functions
    std::vector<Point> createPoints(std::ifstream* pointFile);
    std::vector<std::vector<double>> createNthReferences();
    std::vector<std::vector<double>> createMatrix();
public:
    // constructor & getters / setters
    explicit Graph(std::string inputFile);
    unsigned long getWidth() const;
    std::string getPointFileName();

    // public functions
    bool isVisited(int pointNumber);
    void setVisited(int pointNumber, bool value);
    double getNthEdge(int pointNum, int n);
    double distance(int p1, int p2);
    int getPointId(int n);

    // iterator methods
    std::vector<std::vector<double>>::iterator matrixBegin();
    std::vector<std::vector<double>>::iterator matrixEnd();
    std::vector<Point>::iterator pointsBegin();
    std::vector<Point>::iterator pointsEnd();
};

#endif //C_GRAPH_HPP
