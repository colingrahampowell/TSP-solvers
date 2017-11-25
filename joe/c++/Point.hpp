//
// Created by Joe Pfohl on 11/25/17.
//

#ifndef C_POINT_HPP
#define C_POINT_HPP


class Point
{
private:

    int id;
    int x;
    int y;
    bool visited;

public:
    int getId() const;

    Point(int id, int x, int y);

    void setId(int id);

    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);

    bool isVisited() const;

    void setVisited(bool visited);

    double distanceTo(Point p);
};


#endif //C_POINT_HPP
