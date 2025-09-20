#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <functional>
#include "polygon.hpp"
#include "commands.hpp"
int main(int argc, char **argv)
{
  using namespace evstyunichev;
  using istreamIt = std::istream_iterator< Polygon >;

  if (argc != 2)
  {
    std::cerr << "Incorrect parameters\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  std::ifstream fin(argv[1]);
  while (!fin.eof())
  {
    std::copy(istreamIt(fin), istreamIt(), std::back_inserter(polygons));
    if (!fin)
    {
      fin.clear(fin.rdstate() ^ std::ios::failbit);
      fin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(areaCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"] = std::bind(maxCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"] = std::bind(minCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(countCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["PERMS"] = std::bind(permsCommand, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}