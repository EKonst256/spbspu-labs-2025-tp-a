#ifndef DATA_INPUT_HPP
#define DATA_INPUT_HPP

#include <string>
#include <complex>

namespace evstyunichev
{
  struct DelimiterIO
  {
    char exp;
  };

  struct DelimitersIO
  {
    std::string exps;
  };

  std::istream & operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream & operator>>(std::istream &in, DelimitersIO &&dest);
}

#endif
