#include <iostream>
#include <random>
#include <set>
#include <map>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <typeinfo>
#include <memory.h>
#include "parser.h"

#define LSTRING 100
using namespace std;

struct FlowRecord
{
	uint64_t lastCome;
	uint64_t firstCome;
	int curBatchSize;
	FlowRecord(uint64_t _lastCome, uint64_t _firstCome)
	{
		lastCome = _lastCome;
		firstCome = _firstCome;
		curBatchSize = 1;
	}
};

int main(int argc, char *argv[])
{

	if (argc != 5)
	{
		printf("Wrong Input!\n");
		return 0;
	}

	/* File to be marked */
	char normalFlowDir[LSTRING] = "../data/mix/2to4_10000000_250_1000.dat";
	
	/* Ground Truth */
	char attackListDir[LSTRING] = "";
	sprintf(attackListDir, "../data/mix/testList_2to4_10000000_250_1000.dat");

	double LT = (double)atoi(argv[1]);
	double RT = (double)atoi(argv[2]);
	double minDuration = (double)atoi(argv[3]);
	double dataLen = 80000000;
	uint64_t minBatchInterval;
	int maxBatchSize = atoi(argv[4]);

	//double LTlist[6] = { 1000000, 2000000, 4000000, 8000000, 16000000, 32000000 };
	//double RTlist[6] = { 2000000, 4000000, 8000000, 16000000, 32000000, 64000000 };
	//double minDurationlist[6] = { 30000000, 60000000, 120000000, 240000000, 400000000, 1250000000 };

	FILE *fnrm;
	Packet *normalPacket = new Packet;
	map<uint64_t, FlowRecord> FlowRecordMap;
	map<uint64_t, FlowRecord>::iterator FlowRecordMapIt;
	map<uint64_t, Packet> APTMap;
	map<uint64_t, Packet>::iterator APTMapIt;

	minBatchInterval = (uint64_t)(LT / 5);
	fnrm = fopen(normalFlowDir, "rb");
	FlowRecordMap.clear();
	for (int i = 1; i ; ++i)
	{
		if (i == 9999)
			printf("Reading packet %d ...\n", i + 1);
		else if (i % 10000 == 9999)
			printf("\033[1A\033[KReading packet %d ...\n", i + 1);
		if (!ExtractFormattedPacket(fnrm, normalPacket))
			break;
		FlowRecordMapIt = FlowRecordMap.find(normalPacket->id);
		if (FlowRecordMapIt == FlowRecordMap.end())
		{
			FlowRecordMap.insert(pair<uint64_t, FlowRecord>(normalPacket->id, FlowRecord(normalPacket->us, normalPacket->us)));
			//printf("%llu\n", normalPacket->id);
		}
		else
		{
			uint64_t interval = normalPacket->us - FlowRecordMapIt->second.lastCome;

			if (interval < minBatchInterval)
			{
				FlowRecordMapIt->second.curBatchSize += 1;
				if (FlowRecordMapIt->second.curBatchSize > maxBatchSize)
					FlowRecordMapIt->second.firstCome = 0;
				continue;
			}

			FlowRecordMapIt->second.lastCome = normalPacket->us;
			FlowRecordMapIt->second.curBatchSize = 1;

			if (FlowRecordMapIt->second.firstCome == 0)
			{
				FlowRecordMapIt->second.firstCome = normalPacket->us;
				continue;
			}

			if (interval < LT || interval > RT)
			{
				FlowRecordMapIt->second.firstCome = 0;
				continue;
			}

			if (FlowRecordMapIt->second.lastCome - FlowRecordMapIt->second.firstCome >= minDuration)
			{
				normalPacket->us = FlowRecordMapIt->second.firstCome;
				APTMap.insert(pair<uint64_t, Packet>(FlowRecordMapIt->first, *normalPacket));
			}
		}
			
	}
	fclose(fnrm);

	printf("Writing to list ...\n");

	FILE *flist = fopen(attackListDir, "wb");
	for (APTMapIt = APTMap.begin(); APTMapIt != APTMap.end(); ++APTMapIt)
		WriteFormattedPacket(flist, &(APTMapIt->second));

	fclose(flist);

	printf("Done. Found %d APTs in %d flows\n", APTMap.size(), FlowRecordMap.size());
	
	return 0;
}
