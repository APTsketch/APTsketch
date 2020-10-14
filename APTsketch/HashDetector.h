#ifndef _HASHDETECTOR_H_
#define _HASHDETECTOR_H_

#include <algorithm>
#include <set>
#include <map>
#include "cell.h"
#include "BOBHash32.h"

using namespace std;


class HashDetector
{
public:
  uint32_t windowSize;
  uint64_t winStart;
  double threshold;
  double LT;
  map<uint64_t, Cell> hashMap;
	BOBHash32 bobhash;


	HashDetector(uint32_t _bucketNum, uint32_t _cellNum, uint32_t _windowSize, double _LT)
	{
    windowSize = _windowSize;
    winStart = 0;
    threshold = 0.0;
    LT = _LT;
	  bobhash.initialize(1001);
	}

	void insert(uint64_t id, uint64_t us)
	{
    while (us > winStart + (uint64_t)windowSize)
      winStart += (uint64_t)windowSize;

    pair<map<uint64_t, Cell>::iterator, bool> res = hashMap.insert(pair<uint64_t, Cell>(id, Cell(id, us, (double)us)));
    
    if (!res.second)
    {
      ++res.first->second.frequency;
      if (res.first->second.lastUS <= winStart)
      {
        ++res.first->second.window; 
        double curPeriod = (double)(us - res.first->second.lastUS); 

        if (curPeriod >= LT && curPeriod <= LT * RANGE)
          res.first->second.variance += 1.0;
        else
          res.first->second.variance -= 1.0;
      }
      res.first->second.lastUS = us;
    }
  }

  bool query(uint64_t id)
  {
    map<uint64_t, Cell>::iterator it = hashMap.find(id);
    //printf("%d %lf %lf %lf\n", it->second.window, (double)it->second.lastUS / 60000000.0, it->second.variance, it->second.getWeight());
    return it->second.variance >= threshold;
  }

  double getSuspect(int k)
  {
    set<Cell> cellSet;
    for(map<uint64_t, Cell>::iterator mapIt = hashMap.begin(); mapIt != hashMap.end(); ++mapIt)
    {
      if (mapIt->second.variance > k)
        cellSet.insert(mapIt->second);
    }

    threshold = k;
    return (double)cellSet.size();
  }

	
};
#endif