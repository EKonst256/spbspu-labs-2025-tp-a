#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>

namespace evstyunichev
{
  struct Point
  {
    int x, y;
    friend bool operator<(const Point &p1, const Point &p2);
    friend bool operator==(const Point &p1, const Point &p2);
  };

  struct Polygon
  {
    std::vector< Point > points;
    friend bool operator==(const Polygon &p1, const Polygon &p2);
  };

  std::istream & operator>>(std::istream &, Point &);
  std::istream & operator>>(std::istream &, Polygon &d);

  double getArea(const Polygon &p);
}

#endif
