#include <stdio.h>
#include <algorithm>
#include <set>
#include <sys/time.h>
#include "Detector.h"
#include "HashDetector.h"
#include "./data/parser.h"

using namespace std;

#define DETECTOR Detector

#define LSTRING 100
#define BUCKETSIZE 20
#define RBUCKETSIZE 9
#define SKETCHNUM 3
#define BASETIME 1000000
#define LT 2000000
#define RT 4000000
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
	srand(time(NULL));
	uint32_t bucketNum = (uint32_t)atoi(argv[1]);
    uint32_t cellNum = (uint32_t)atoi(argv[2]);
    uint32_t windowSize = (uint32_t)(LT / 5);

	FILE *ftest = fopen(testDir, "rb");	
	Packet *packet = new Packet;

	DETECTOR *detector[SKETCHNUM];
	for (int i = 0; i < SKETCHNUM; ++i)
		detector[i] = new DETECTOR(bucketNum, windowSize, LT, RT, rand() % 2000, OUTTHRES);
	
	HashTable *hashtable = new HashTable(bucketNum, 1001);

	int packetNum = 0;
	double lastTime = 0;
	double insertTime = 0.0;
	bool get_result = false;
	timeval startTime, endTime;
	

	while (ExtractFormattedPacket(ftest, packet))
	{
		
		++packetNum;

		get_result = false;
		if (packetNum == 100000)
			printf("Reading packet %d ...\n", packetNum);
		else if (packetNum % 100000 == 0)
			printf("\033[1A\033[KReading packet %d ...\n", packetNum);
		
		for (int i = 0; i < SKETCHNUM; ++i)
		{
			gettimeofday(&startTime, NULL);
			get_result |= (detector[i]->insert(packet->id, packet->us));		
			gettimeofday(&endTime, NULL);
			insertTime += ((double)endTime.tv_sec * 1000000 + (double)endTime.tv_usec) - ((double)startTime.tv_sec * 1000000 + (double)startTime.tv_usec);
		}
		
		lastTime = (double)(packet->us);
		if (get_result)
			hashtable->insert(packet->id);
	}

	printf("\033[1A\033[K\nRead file %s\n", testDir);
	printf("%d packets over %ds, mem %dKB\n", packetNum, (int)(lastTime / 1000000), (SKETCHNUM * bucketNum * BUCKETSIZE + bucketNum * RBUCKETSIZE) / 1000);

	fclose(ftest);

	int totalSuspect = hashtable->getSuspect();
	
	//detector->print();

	FILE *flist = fopen(attackListDir, "rb");
	int totalHit = 0;
	int all = 0;

	while (ExtractFormattedPacket(flist, packet))
	{
		if (hashtable->query(packet->id))
			++totalHit;	
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
