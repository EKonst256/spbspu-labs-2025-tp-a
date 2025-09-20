#include "polygon.hpp"
#include "algorithm"
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
#include "data_input.hpp"
#include "stream_guard.hpp"

namespace
{
  int GaussTermSumm(const evstyunichev::Point &first, const evstyunichev::Point &second)
  {
    return (first.x * second.y - first.y * second.x);
  }
}

namespace evstyunichev
{
  std::istream & operator>>(std::istream &in, Point &point)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    StreamGuard guard(in);
    in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' } >> point.y >> DelimiterIO{ ')' };
    return in;
  }

  std::istream & operator>>(std::istream &in, Polygon &p)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    StreamGuard guard(in);
    size_t sz = 0;
    if (!(in >> sz) || sz < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::vector< Point > temp(sz);
    std::copy_n(std::istream_iterator< Point >(in), sz, temp.begin());
    if (in)
    {
      p.points = std::move(temp);
    }
    return in;
  }

  double getArea(const Polygon &p)
  {
    std::vector< int > sub{ GaussTermSumm(p.points.front(), p.points.back()) };
    std::transform
    (
      p.points.cbegin() + 1,
      p.points.end(),
      p.points.cbegin(),
      std::back_inserter(sub),
      GaussTermSumm
    );
    return (abs(std::accumulate(sub.cbegin(), sub.cend(), 0.0)) / 2.0);
  }

  bool operator<(const Point &p1, const Point &p2)
  {
    return ((p1.x < p2.x) ? 1 : ((p1.x == p2.x) ? (p1.y < p2.y) : 0));
  }

  bool operator==(const Point &p1, const Point &p2)
  {
    return ((p1.x == p2.x) && (p1.y == p2.y));
  }

  bool operator==(const Polygon &p1, Polygon &p2)
  {
    return (p1.points == p2.points);
  }
}
