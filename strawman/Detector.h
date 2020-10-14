#ifndef _DETECTOR_H_
#define _DETECTOR_H_

#include <cstdint>
#include <algorithm>
#include "BOBHash32.h"

using namespace std;

struct Bucket
{
  uint32_t frequency;
  uint64_t lastUS;

  double variance;
  Bucket()
  {
    frequency = 0;
    lastUS = 0;
    variance = 0;
  }
};

class Detector
{
public:
	uint32_t bucketNum;
  uint32_t windowSize;
  uint64_t winStart;
  double threshold;
  double LT, RT;
  Bucket* bucket;
	BOBHash32 bobhash;


	Detector(uint32_t _bucketNum, uint32_t _windowSize, double _LT, double _RT, uint _bobinit, double _threshold)
	{
	  bucketNum = _bucketNum;
    windowSize = _windowSize;
    winStart = 0;
    LT = _LT;
    RT = _RT;
    bucket = new Bucket[bucketNum];
	  bobhash.initialize(_bobinit);
    threshold = _threshold;
	}

  ~Detector()
  {
    delete [] bucket;
  }

	bool insert(uint64_t id, uint64_t us)
	{ 
  	// Map a key to a fingerprint

		int pos = bobhash.run((char *)&id, 8) % bucketNum;

    while (us >= winStart + (uint64_t)windowSize)
      winStart += (uint64_t)windowSize;
    
    if (bucket[pos].lastUS >= winStart)
    {
      ++bucket[pos].frequency;
      if (bucket[pos].frequency > 10)
        bucket[pos].variance = 0;
    }
    else
    {
      bucket[pos].frequency = 1;
      double curPeriod = (double)(us - bucket[pos].lastUS);
      if (curPeriod >= LT && curPeriod <= RT)
        bucket[pos].variance += 1;
      else
        bucket[pos].variance -= 1;
    }
    
    bucket[pos].lastUS = us;
    return (bucket[pos].variance >= threshold);
	}
  
};

struct RBucket {
  uint64_t id;
  bool inserted;
  RBucket()
  {
    id = 0;
    inserted = false;
  }

  bool operator<(const RBucket &bucket) const {
    return id < bucket.id;
  }
};

class HashTable
{
public:
  uint32_t bucketNum;
  RBucket* bucket;
	BOBHash32 bobhash;

  HashTable(uint32_t _bucketNum, uint _bobinit){
    bucketNum = _bucketNum;
    bucket = new RBucket[bucketNum];
    bobhash.initialize(_bobinit);
  }

  ~HashTable(){
    delete [] bucket;
  }

  void insert(uint64_t id) {
    int pos = bobhash.run((char *)&id, 8) % bucketNum;
    bucket[pos].id = id;
    bucket[pos].inserted = true;
  }

  bool query(uint64_t id)
  {
    for (int i = 0; i < bucketNum; ++i)
    {
      if (bucket[i].id == id && bucket[i].inserted)
        return true;
    }
    return false;
  }

  int getSuspect()
    {
      set<RBucket> cellSet;
      for (int i = 0; i < bucketNum; ++i)
      {
        if (bucket[i].inserted)
          cellSet.insert(bucket[i]);
      }
      return (double)cellSet.size();
    }

};
#endif