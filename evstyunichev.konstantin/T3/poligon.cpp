#include "poligon.hpp"
#include "algorithm"
#include <iostream>
#include <iterator>
#include <vector>
#include "../common/data_input.hpp"
#include "../common/stream_guard.hpp"

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

std::istream & evstyunichev::operator>>(std::istream &in, Poligon &poligon)
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
    poligon.points = std::move(temp);
  }
  return in;
}
