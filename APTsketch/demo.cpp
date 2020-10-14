#include <stdio.h>
#include <algorithm>
#include <set>
#include <sys/time.h>
#include "Detector.h"
#include "./data/parser.h"

using namespace std;

#define DETECTOR Detector

#define LSTRING 100
#define SKETCHNUM 1
#define BASETIME 1000000

#define LT 2000000
#define RT 4000000
#define MinKeep 4000000
#define OUTTHRES (15.0)

char resultDir[LSTRING] = "";
char testDir[LSTRING] = "../data/mix/2to4_10000000_250_1000.dat";
char attackListDir[LSTRING] = "../data/mix/testList_2to4_10000000_250_1000.dat";

int main(int argc, char const *argv[])
{
	if (argc != 3)
	{
		printf("./a.out _bucketNum _cellNumn");
		return 0;
	}

	uint32_t bucketNum = (uint32_t)atoi(argv[1]);
    uint32_t cellNum = (uint32_t)atoi(argv[2]);
    uint32_t windowSize = (uint32_t)(LT / 5);
	
	double LTlist[6] = { 1000000, 2000000, 4000000, 8000000, 16000000, 32000000 };
	double RTlist[6] = { 2000000, 4000000, 8000000, 16000000, 32000000, 64000000 };
	double Thres[6] = { 15.0, 15.0, 15.0, 15.0, 13.0, 20.0 };

	Packet *packet = new Packet;
	
	FILE *ftest = fopen(testDir, "rb");	
	DETECTOR *detector[SKETCHNUM];
	for (int i = 0; i < SKETCHNUM; ++i)
	{
		detector[i] = new DETECTOR(bucketNum, cellNum, windowSize, LT, RT, MinKeep);
		//detector[i] = new DETECTOR(bucketNum, cellNum, (uint32_t)(LTlist[i] / 5), LTlist[i], RTlist[i], RTlist[i]);
	}
	
	int packetNum = 0;
	double lastTime = 0;
	double insertTime = 0.0;
	timeval startTime, endTime;

	while (ExtractFormattedPacket(ftest, packet))
	{
		++packetNum;
		if (packetNum == 100000)
			printf("Reading packet %d ...\n", packetNum);
		else if (packetNum % 100000 == 0)
			printf("\033[1A\033[KReading packet %d ...\n", packetNum);

		for (int i = 0; i < SKETCHNUM; ++i)
		{
			gettimeofday(&startTime, NULL);
			detector[i]->insert(packet->id, packet->us);
			gettimeofday(&endTime, NULL);
			insertTime += ((double)endTime.tv_sec * 1000000 + (double)endTime.tv_usec) - ((double)startTime.tv_sec * 1000000 + (double)startTime.tv_usec);
		}
		lastTime = (double)(packet->us);
	}
	fclose(ftest);

	printf("\033[1A\033[K\nRead file %s\n", testDir);
	printf("%d packets over %ds, mem %dKB\n", packetNum, (int)(lastTime / 1000000), bucketNum * cellNum * CELLSIZE / 1000);
	

	int totalSuspect = 0;
	int suspect[SKETCHNUM] = { 0 };
	set<uint64_t> APTset;
	for (int i = 0; i < SKETCHNUM; ++i)
	{
		suspect[i] = detector[i]->getSuspect(OUTTHRES);//lastTime / (2.0 * RANGE * BASETIME * (1 << i)));
		totalSuspect += suspect[i];
		//detector[i]->getSuspectItems(APTset, Thres[i]);
	}
	//totalSuspect = APTset.size();
	//detector->print();

	FILE *flist = fopen(attackListDir, "rb");
	int totalHit = 0;
	int all = 0;
	int hit[SKETCHNUM] = { 0 };

	while (ExtractFormattedPacket(flist, packet))
	{
		for (int i = 0; i < SKETCHNUM; ++i)
		{
			if (detector[i]->query(packet->id))
			{
				++totalHit;
				++hit[i];
				break;
			}	
		}
		++all;
	}
	fclose(flist);

	//detector[0]->print();
	
	printf("Hit: %d\n", totalHit);
	printf("Suspect: %d\n", totalSuspect);
	printf("ALL: %d\n", all);
	//for (int i = 0; i < SKETCHNUM; ++i)
		//printf("%d-%ds: %d / %d\n", LT/1000000, RT/1000000, hit[i], suspect[i]);
	printf("Precision: %lf\n", (double)totalHit / totalSuspect);
	printf("Recall: %lf\n", (double)totalHit / all);
	printf("Mips: %lf\n", (double)packetNum / insertTime);

	return 0;
}
