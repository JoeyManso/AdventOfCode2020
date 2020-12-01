#ifndef POINT2D_H
#define POINT2D_H

struct Point2D
{
    int x;
    int y;

    Point2D() :
        x(0), y(0)
    {}
    Point2D(int inX, int inY) :
        x(inX), y(inY)
    {}

    bool IsWithinBounds(const int xMax, const int yMax) const
    {
        return(x >= 0 && y >= 0 && x < xMax && y < yMax);
    }
    bool operator==(const Point2D& p) const
    {
        return(x == p.x && y == p.y);
    }
    bool operator!=(const Point2D& p) const
    {
        return(x != p.x || y != p.y);
    }
    bool operator<(const Point2D& p) const
    {
        return y == p.y ? x < p.x : y < p.y;
    }
    Point2D operator+(const Point2D& p) const
    {
        return(Point2D(x + p.x, y + p.y));
    }
};

#endif // POINT2D_H