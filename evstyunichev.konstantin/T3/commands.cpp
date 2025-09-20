#include "commands.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <map>
#include <functional>
#include <iomanip>
#include <vector>
#include <numeric>
#include <limits>
#include "polygon.hpp"
#include "stream_guard.hpp"

namespace
{
  struct equalTo
  {
    const evstyunichev::Polygon &p1;
    bool operator()(const evstyunichev::Polygon &p2)
    {
      return (p1 == p2);
    }
  };

  struct numOfVertexPred
  {
    size_t sz;
    bool operator()(const evstyunichev::Polygon &p)
    {
      return p.points.size() == sz;
    }
  };

  bool isEven(const evstyunichev::Polygon &p)
  {
    return !isOdd(p);
  }

  bool isOdd(const evstyunichev::Polygon &p)
  {
    return p.points.size() % 2;
  }

  bool allPolygons(const evstyunichev::Polygon &p)
  {
    return true;
  }

  template< typename Pred >
  double areaSumm(const std::vector< evstyunichev::Polygon > &polygons, Pred p)
  {
    std::vector< double > needed;
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(needed), p);
    std::vector< double > areas;
    std::transform(filtered.cbegin(), filtered.cend(), std::back_inserter(areas), evstyunichev::getArea);
    return std::accumulate(areas.cbegin(), areas.cend(), 0.0);
  }

  double areaEven(const std::vector< evstyunichev::Polygon > &polygons)
  {
    return areaSumm(polygons, isEven);
  }

  double areaOdd(const std::vector< evstyunichev::Polygon > &polygons)
  {
    return areaSumm(polygons, isOdd);
  }

  double areaMean(const std::vector< evstyunichev::Polygon > &polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No polygons");
    }
    return areaSumm(polygons, allPolygons) / polygons.size();
  }

  double areaNum(const std::vector< evstyunichev::Polygon > &polygons, size_t n)
  {
    return areaSumm(polygons, numOfVertexPred{ n });
  }

  bool vertexComp(const evstyunichev::Polygon &p1, const evstyunichev::Polygon &p2)
  {
    return p1.points.size() < p2.points.size();
  }

  bool areaComp(const evstyunichev::Polygon & p1, const evstyunichev::Polygon &p2)
  {
    return evstyunichev::getArea(p1) < evstyunichev::getArea(p2);
  }

  void maxVertex(const std::vector< evstyunichev::Polygon > &polygons, std::ostream &out)
  {
    auto mx = (*std::max_element(polygons.cbegin(), polygons.cend(), vertexComp));
    out << mx.points.size() << '\n';
  }

  void maxArea(const std::vector< evstyunichev::Polygon > &polygons, std::ostream &out)
  {
    auto mx = (*std::max_element(polygons.cbegin(), polygons.cend(), areaComp));
    evstyunichev::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << evstyunichev::getArea(mx) << '\n';
  }

  void minArea(const std::vector< evstyunichev::Polygon > &polygons, std::ostream &out)
  {
    auto mn = (*std::min_element(polygons.cbegin(), polygons.cend(), areaComp));
    evstyunichev::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << evstyunichev::getArea(mn) << '\n';
  }

  void minVertex(const std::vector< evstyunichev::Polygon > &polygons, std::ostream &out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), vertexComp));
    out << min.points.size() << '\n';
  }

  template< typename Predicate >
  size_t countIf(const std::vector< evstyunichev::Polygon > &polygons, Predicate pred)
  {
    std::vector< evstyunichev::Polygon > needed;
    std::copy_if(polygons.cbegin(), polygons.end(), std::back_inserter(needed), pred);
    return needed.size();
  }

  void countEven(const std::vector< evstyunichev::Polygon > &polygons, std::ostream &out)
  {
    out << countIf(polygons, isEven) << '\n';
  }

  void countOdd(const std::vector< evstyunichev::Polygon > &polygons, std::ostream &out)
  {
    out << countIf(polygons, isOdd) << '\n';
  }

  void countNum(const std::vector< evstyunichev::Polygon > &polygons, std::ostream &out, size_t n)
  {
    out << countIf(polygons, numOfVertexPred{ n }) << '\n';
  }

  void countPerms(const std::vector< evstyunichev::Polygon > &polygons, const evstyunichev::Polygon &p, std::ostream &out)
  {
    evstyunichev::Polygon cur;
    std::copy(p.points.begin(), p.points.end(), cur.points.begin());
    std::sort(cur.points.begin(), cur.points.end());
    out << countPermsSub(polygons, cur) << '\n';
  }

  size_t countPermsSub(const std::vector< evstyunichev::Polygon > &polygons, evstyunichev::Polygon &p, size_t ans = 0)
  {
    size_t cur = std::count_if(polygons.cbegin(), polygons.cend(), equalTo{ p });
    ans += cur;
    if (!std::next_permutation(p.points.begin(), p.points.end()))
    {
      return cur;
    }
    return countPermsSub(polygons, p, ans);
  }
}

void evstyunichev::areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string subcommand;
  in >> subcommand;
  double ans = 0.0;
  std::map< std::string, std::function< double() > > subcommands;
  subcommands["EVEN"] = std::bind(areaEven, polygons);
  subcommands["ODD"] = std::bind(areaOdd, polygons);
  subcommands["MEAN"] = std::bind(areaMean, polygons);
  try
  {
    ans = subcommands.at(subcommand)();
  }
  catch (...)
  {
    size_t n = std::stoull(subcommand);
    if (n < 3)
    {
      throw std::logic_error("Few vertices");
    }
    ans = areaNum(polygons, n);
  }
  evstyunichev::StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << ans << '\n';
}

void evstyunichev::maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string subcommand;
  in >> subcommand;
  if (polygons.empty())
  {
    throw std::logic_error("No polygons");
  }
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(maxArea, polygons, out);
  subcommands["VERTEXES"] = std::bind(maxVertex, polygons, out);
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("Unknown command");
  }
}

void evstyunichev::minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string subcommand;
  in >> subcommand;
  if (polygons.empty())
  {
    throw std::logic_error("No polygons");
  }
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(minArea, polygons, out);
  subcommands["VERTEXES"] = std::bind(minVertex, polygons, out);
}

void evstyunichev::countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > subcommands;
  subcommands["EVEN"] = std::bind(countEven, polygons, out);
  subcommands["ODD"] = std::bind(countOdd, polygons, out);
  try
  {
    subcommands.at(subcommand)();
  }
  catch (...)
  {
    size_t n = std::stoull(subcommand);
    if (n < 3)
    {
      throw std::logic_error("Not enough vertixes");
    }
    countNum(polygons, out, n);
  }
}

void evstyunichev::permsCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  Polygon p, cur;
  in >> p;
  if (!in)
  {
    throw std::logic_error("Invalid Polygon");
  }
}
