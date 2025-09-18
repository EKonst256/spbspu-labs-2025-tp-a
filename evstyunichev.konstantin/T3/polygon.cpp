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
  struct CalcAreaTerm
  {
    double operator()(const evstyunichev::Point &p1, const evstyunichev::Point &p2)
    {
      return p1.x * p2.y - p2.x * p1.y;
    }
  };

  double dist(const evstyunichev::Point &p1, const evstyunichev::Point &p2)
  {
    return std::sqrt(std::pow((p1.x - p2.x), 2) + std::pow((p1.y - p2.y), 2));
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
  const auto points = polygon.points;
  const Point first = points.front();
  const Point last = points.back();
  double area = std::inner_product(
    points.begin(), points.end() - 1, points.begin() + 1, CalcAreaTerm()(last, first), std::plus< double >(), CalcAreaTerm());
  return std::abs(area) / 2.0;
}
