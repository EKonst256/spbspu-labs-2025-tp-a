#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "polygon.hpp"
#include <iosfwd>
#include <vector>
namespace evstyunichev
{
  void areaCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void maxCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void minCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void countCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void permsCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
  void echoCommand(std::istream &, std::ostream &, const std::vector< Polygon > &);
}
#endif
