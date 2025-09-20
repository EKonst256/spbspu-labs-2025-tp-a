#include "data_input.hpp"
#include <iomanip>
#include <ios>
#include <iostream>
#include <utility>
#include "stream_guard.hpp"

std::istream & evstyunichev::operator>>(std::istream &in, DelimiterIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, DelimitersIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);
  std::noskipws(in);
  for (char c: dest.exps)
  {
    in >> DelimiterIO{ c };
  }
  return in;
}
