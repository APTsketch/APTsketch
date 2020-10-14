#ifndef _DETECTOR_H_
#define _DETECTOR_H_

#include <algorithm>
#include "BOBHash32.h"
#include "cell.h"
#include <cmath>

using namespace std;

struct Bucket
{
  Cell *cell;
  Bucket()
  {
    cell = new Cell[CELLNUM];
  }
  ~Bucket()
  {
    delete [] cell;
  }
};

class Detector
{
public:
	uint32_t bucketNum;
	uint32_t cellNum;
  uint32_t windowSize;
  uint64_t winStart;
  double threshold;
  double LT, RT, minKeep;
  Bucket* bucket;
	BOBHash32 bobhash;
  // double currentFrequency;
  // double lastFrequency;

	Detector(uint32_t _bucketNum, uint32_t _cellNum, uint32_t _windowSize, double _LT, double _RT, double _minKeep)
	{
	  
	  bucketNum = _bucketNum;
    cellNum = _cellNum;
    windowSize = _windowSize;
    winStart = 0;
    // currentFrequency = lastFrequency = 0.0;
    LT = _LT;
    RT = _RT;
    minKeep = _minKeep;
    bucket = new Bucket[bucketNum];
	  bobhash.initialize(1001);
	}

  ~Detector()
  {
    delete [] bucket;
  }

	bool insert(uint64_t id, uint64_t us)
	{ 
  	// Map a key to a fingerprint

		int pos = bobhash.run((char *)&id, 8) % bucketNum;
		int minIndex = -1;
		double minWeight = (double)(0x7fffffff);

    /*
    if (us >= winStart + (uint64_t)windowSize) {
      lastFrequency = currentFrequency;
      currentFrequency = 0;
    }
    currentFrequency += 1.0;
    */
    while (us >= winStart + (uint64_t)windowSize)
      winStart += (uint64_t)windowSize;
    
    for (int i = 0; i < cellNum; ++i)
    {
      // Accumulate
      if (id == bucket[pos].cell[i].id)
      {
        if (bucket[pos].cell[i].lastUS >= winStart)
        {
          ++bucket[pos].cell[i].frequency;
          if (bucket[pos].cell[i].frequency > 10)
          {
            bucket[pos].cell[i].variance -= 10000.0;
            return true;
          }
        }
        else
        {
          bucket[pos].cell[i].frequency = 1;
          //++bucket[pos].cell[i].window; 

          double curPeriod = (double)(us - bucket[pos].cell[i].lastUS); 
          if (curPeriod >= LT && curPeriod <= RT)
            bucket[pos].cell[i].variance += 1.0;
          else
            bucket[pos].cell[i].variance -= 1.0;
        }

        bucket[pos].cell[i].lastUS = us;
        return true;
      }
    }

		// Search the key in the bucket
    for (int i = 0; i < cellNum; ++i)
		{
      // Insert
			if (bucket[pos].cell[i].variance < 0.0)
			{
				bucket[pos].cell[i].id = id;
				bucket[pos].cell[i].frequency = 1;
				//bucket[pos].cell[i].window = 1;
        bucket[pos].cell[i].lastUS = us;
        bucket[pos].cell[i].firstComeUS = us;
        bucket[pos].cell[i].variance = 0.0;
        return true;
			}
	 	}

    // Find which to substitute
    for (int i = 0; i < cellNum; ++i)
    {
      double weight = bucket[pos].cell[i].getWeight();
      if (weight < minWeight && us - bucket[pos].cell[i].lastUS > minKeep && us - bucket[pos].cell[i].firstComeUS > RT)
      {
        minIndex = i;
        minWeight = weight;
      }
    }

  	// Substitute
    if (minIndex < 0)
      return false;

    double poss = 1.0 / exp(minWeight);
  	if ((double)rand() / (double)RAND_MAX < poss)
  	{
  		bucket[pos].cell[minIndex].id = id;
      bucket[pos].cell[minIndex].frequency = 1;
      //bucket[pos].cell[minIndex].window = 1;
      bucket[pos].cell[minIndex].variance += 1.0; // ???
      bucket[pos].cell[minIndex].lastUS = us;
      bucket[pos].cell[minIndex].firstComeUS = us;
      return true;
  	}
    
    return false;
	}

  void print()
  {
    for (int i = 0; i < bucketNum; ++i)
    {
      for (int j = 0; j < cellNum; ++j)
      {
        //printf("%d %d %lf", bucket[i].cell[j].frequency, bucket[i].cell[j].window, bucket[i].cell[j].variance);
        if (bucket[i].cell[j].id == 0)
          printf("+\n");
        else
          printf("\n");
      }
      printf("---\n");
    }
  }

  bool query(uint64_t id)
  {
    for (int i = 0; i < bucketNum; ++i)
    {
      for (int j = 0; j < cellNum; ++j)
      {
          if (bucket[i].cell[j].id == id)
          {
            if (bucket[i].cell[j].variance >= threshold)
            {
              bucket[i].cell[j].id = 0;
              return true;
            }
            else
              return false;
          }
      }
    }
    return false;
  }


  int getSuspect(double k)
  {
    set<Cell> cellSet;
    for (int i = 0; i < bucketNum; ++i)
    {
      for (int j = 0; j < cellNum; ++j)
      {
        if (bucket[i].cell[j].variance >= k)
          cellSet.insert(bucket[i].cell[j]);
      }
    }

    threshold = k;
    return (double)cellSet.size();
  }

  void getSuspectItems(set<uint64_t>& APTset, double k)
  {
    for (int i = 0; i < bucketNum; ++i)
    {
      for (int j = 0; j < cellNum; ++j)
      {
        if (bucket[i].cell[j].variance >= k)
          APTset.insert(bucket[i].cell[j].id);
      }
    }
    threshold = k;
  }
};
#endif