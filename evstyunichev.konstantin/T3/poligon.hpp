#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>

namespace evstyunichev
{
  struct Point
  {
    int x, y;
  };

  struct Poligon
  {
    std::vector< Point > points;
  };

  std::istream & operator>>(std::istream &in, Point &dest);
  std::istream & operator>>(std::istream &in, Poligon &dest);\
}

#endif
