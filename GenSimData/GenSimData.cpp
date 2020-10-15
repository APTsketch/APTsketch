#include <iostream>
#include <random>
#include <set>
#include <stdlib.h>
#include <time.h>
#include <typeinfo>
#include <memory.h>
#include "parser.h"
#include "genzipf.h"

#define LSTRING 100
#define US(attackPacket) ((uint64_t)((double)(attackPacket->us - attackStartUs) * variation) + offset)
#define ID(attackPacket) (attackPacket->id + i)
using namespace std;

int main(int argc, char *argv[])
{

	if (argc != 7)
	{
		printf("Wrong Input!\n");
		return 0;
	}

	double alpha_zipf = ((double)atoi(argv[1])) / ((double)atoi(argv[2]));
	int n_zipf = atoi(argv[3]);
	int flowNum = atoi(argv[4]);
	int intv = atoi(argv[5]);
	int pktNum = atoi(argv[6]);

	vector<uint64_t> flowPool;

	srand((unsigned)time(NULL));  
  	rand_val((unsigned)time(NULL));

  	for (int i = 0; i < flowNum; ++i)
  	{
  		double freq = zipf(alpha_zipf, n_zipf);
  		for (int j = 0; j < freq; ++j)
  			flowPool.push_back(i);
  	}

  	int flowPoolSize = flowPool.size();
  	char bgDir[LSTRING] = "";
	sprintf(bgDir, "../data/normal/simbg_%f_%d_%d_%d_%d.dat", alpha_zipf, n_zipf, flowNum, intv, pktNum);

  	FILE *fnrm = fopen(bgDir, "wb");
  	for (int i = 0; i < pktNum; ++i)
	{
		Packet *pkt = new Packet;
		pkt->us = i * intv;
		pkt->id = flowPool[rand() % flowPoolSize];
		WriteFormattedPacket(fnrm, pkt);
	}
  	fclose(fnrm);

  	/*
	// Output to files
	int file_cnt = 0;
	sprintf(sim_data_dir, "./sim/%d_%d_%d_%lf.dat", NUM_OF_MAL_FLOWS, NUM_OF_FLOWS, PKTS_PER_WINDOW, ALPHA);
	FILE *fsrc = fopen(sim_data_dir, "wb");
	printf("Generating... FILE: %s\n", sim_data_dir);

	while(file_cnt < NUM_OF_WIN)
	{
		printf("Generating... WINDOW: %d\n",file_cnt);

		int pkt_cnt = 0;

		// 攻击流按照概率在该窗口内出现 2/2
		for (int i = 0; i < NUM_OF_MAL_FLOWS; ++i)
		{
			double occurrence = (double)rand() / (double)RAND_MAX;
			if (occurrence <= (double)NUM_OF_WIN_PER_MAL / (double)NUM_OF_WIN)
			{
				uint64_t tmp = flowIDs[i];
				int frequency = (rand() % MAX_MAL_PKTS_PER_WIN) + 1;

				while(frequency--)
				{
					fwrite(&tmp, 1, 8, fsrc);
					fwrite(&tmp, 1, 8, fsrc);
					++pkt_cnt;

				}
			}		
		}

		for (int h = pkt_cnt; h < PKTS_PER_WINDOW; ++h)
		{
			int flow = NUM_OF_MAL_FLOWS + zipf(ALPHA, NUM_OF_FLOWS - NUM_OF_MAL_FLOWS);

			uint64_t tmp = flowIDs[flow];
			fwrite(&tmp, 1, 8, fsrc);
			fwrite(&tmp, 1, 8, fsrc);
		}		
		
		file_cnt++;
		printf("\033[1A");
       	printf("\033[K");
	}
	fclose(fsrc);
	printf("\033[1A");
    printf("\033[K");
	printf("FILE: %s Complete.\n", sim_data_dir);
	*/

	return 0;

}
