#ifndef _CELL_H_
#define _CELL_H_

#include <algorithm>
#include <cstdint>

using namespace std;

#define CELLNUM 8
#define RANGE 2.0
#define CELLSIZE 36

struct Cell
{
  uint64_t id;
  uint32_t frequency;
  //uint32_t window; 
  uint64_t lastUS;
  uint64_t firstComeUS;
  //double recentPeriod;
  double variance;
  //double backupVariance;

  Cell()
  {
    id = 0;
    frequency = 0;
    //window = 0;
    lastUS = 0;
    firstComeUS = 0;
    //recentPeriod = 0.0;
    variance = -1.0;
  }
  /*
  Cell(uint64_t _id, uint64_t _us, double _recentPeriod)
  {
    id = _id;
    frequency = 1;
    window = 1;
    lastUS = _us;
    firstComeUS = _us;
    recentPeriod = _recentPeriod;
    variance = 0.0;
  }
  */

  double getWeight()
  {
    return variance;
  }

  bool operator<(const Cell &cell) const
  {
    Cell a = *this;
    Cell b = cell;
    if (a.getWeight() != b.getWeight())
      return a.getWeight() > b.getWeight();
    else
      return id < cell.id;
  }
};
#endif