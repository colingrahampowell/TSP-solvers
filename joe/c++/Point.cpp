//
// Created by Joe Pfohl on 11/25/17.
//

#include <cmath>
#include "Point.hpp"

int Point::getId() const
{
    return id;
}

void Point::setId(int id)
{
    Point::id = id;
}

int Point::getX() const
{
    return x;
}

void Point::setX(int x)
{
    Point::x = x;
}

int Point::getY() const
{
    return y;
}

void Point::setY(int y)
{
    Point::y = y;
}

bool Point::isVisited() const
{
    return visited;
}

void Point::setVisited(bool visited)
{
    Point::visited = visited;
}

double Point::distanceTo(Point p)
{
    double xDist = x - p.getX();
    double yDist = y - p.getY();
    return sqrt(pow(xDist, 2) + pow(yDist, 2));
}

Point::Point(int id, int x, int y) : id(id), x(x), y(y)
{
    setVisited(false);
}
