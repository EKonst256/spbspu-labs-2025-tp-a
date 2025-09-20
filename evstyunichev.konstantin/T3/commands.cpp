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
#include "../common/stream_guard.hpp"

namespace
{
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
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(needed), p);
    std::vector< double > areas;
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), evstyunichev::getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
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

  void maxVertex(const std::vector< evstyunichev::Polygon >& polygons, std::ostream& out)
  {
    auto mx = (*std::max_element(polygons.begin(), polygons.end(), vertexComp));
    out << mx.points.size() << '\n';
  }

  void maxArea(const std::vector< evstyunichev::Polygon > &polygons, std::ostream &out)
  {
    auto mx = (*std::max_element(polygons.begin(), polygons.end(), areaComp));
    evstyunichev::StreamGuard ostr(out);
    out << std::fixed << std::setprecision(1) << evstyunichev::getArea(mx) << '\n';
  }

  void minArea(const std::vector< evstyunichev::Polygon >& polygons, std::ostream& out)
  {
    auto mn = (*std::min_element(polygons.begin(), polygons.end(), areaComp));
    evstyunichev::StreamGuard ostr(out);
    out << std::fixed << std::setprecision(1) << evstyunichev::getArea(mn) << '\n';
  }

  void minVertex(const std::vector< evstyunichev::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), vertexComp));
    out << min.points.size() << '\n';
  }

  template< typename Predicate >
  size_t countIf(const std::vector< evstyunichev::Polygon > &polygons, Predicate pred)
  {
    std::vector< evstyunichev::Polygon > needed;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(needed), pred);
    return needed.size();
  }

  void countEven(const std::vector< evstyunichev::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isEven) << "\n";
  }

  void countOdd(const std::vector< evstyunichev::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isOdd) << "\n";
  }

  void countNum(const std::vector< evstyunichev::Polygon >& polygons, std::ostream& out, size_t n)
  {
    out << countIf(polygons, numOfVertexPred{ n }) << "\n";
  }
}
