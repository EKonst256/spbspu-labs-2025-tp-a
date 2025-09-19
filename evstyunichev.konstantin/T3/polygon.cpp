#include "polygon.hpp"
#include "algorithm"
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
#include "../common/data_input.hpp"
#include "../common/stream_guard.hpp"

namespace
{
  int GaussTermSumm(const evstyunichev::Point &first, const evstyunichev::Point &second)
  {
    return (first.x * second.y - first.y * second.x);
  }
}

std::istream & evstyunichev::operator>>(std::istream &in, Point &point)
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

std::istream & evstyunichev::operator>>(std::istream &in, Polygon &polygon)
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
    polygon.points = std::move(temp);
  }
  return in;
}

double evstyunichev::getArea(const Polygon &polygon)
{
  std::vector< int > sub{ GaussTermSumm(polygon.points.front(), polygon.points.back()) };
  std::transform
  (
    polygon.points.cbegin() + 1,
    polygon.points.end(),
    polygon.points.cbegin(),
    std::back_inserter(sub),
    GaussTermSumm
  );
  return (abs(std::accumulate(sub.cbegin(), sub.cend(), 0.0)) / 2.0);
}
