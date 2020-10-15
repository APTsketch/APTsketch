#include <iostream>
#include <random>
#include <set>
#include <stdlib.h>
#include <time.h>
#include <typeinfo>
#include <memory.h>
#include "parser.h"

#define LSTRING 100
#define INSERTNUM 1
#define US(attackPacket) ((uint64_t)((double)(attackPacket->us - attackStartUs) + offset))
#define ID(attackPacket) (attackPacket->id + i)
using namespace std;

const char comment[LSTRING] = "atk_2to4";

int main(int argc, char *argv[])
{

	if (argc != 3)
	{
		printf("Wrong Input!\n");
		return 0;
	}

	/* Attack File */
	char attackFlowDir[INSERTNUM][LSTRING] = {
		"../data/attack/Taidoor2.pcap"
	};

	int variantNum = atoi(argv[1]);
	uint64_t maxOffset = (uint64_t)atoi(argv[2]);
	int cnt = 0;
	srand((unsigned)time(NULL));

	/* Median File */
	char medFlowDir[LSTRING] = "../data/mix/intermediate.dat";
	FILE *fmed = fopen(medFlowDir, "wb");
	fclose(fmed);

	/* Output File */
	char mixFlowDir[LSTRING] = "";
	sprintf(mixFlowDir, "../data/mix/%s_%d.dat", comment, variantNum);

	Packet *normalPacket = new Packet;
	Packet *attackPacket = new Packet;
	printf("%s\n", comment);

	printf("Copying normal flows...\n");
	
	set<uint64_t> attackList;
	uint64_t datalen = (uint64_t)(5000000000);
	uint64_t currentUs;

	for (int k = 0; k < INSERTNUM; ++k)	
	{
		for (int i = 0; i < variantNum; ++i) // variantNum
		{
			int attackSeq = 0;
			printf("\033[1A\033[KInserting attack flow %d for %d-th time...\n", k, i);
			//double variation = LT / PERIOD + (RT - LT) / PERIOD * ((double)rand() / (double)RAND_MAX);
			//double variation = 1.0 + maxVariation * 2.0 * ((double)rand() / (double)RAND_MAX - 0.5);
			uint64_t offset = (uint64_t)((double)maxOffset * (double)rand() / (double)RAND_MAX);
			FILE *fatk = fopen(attackFlowDir[k], "rb");
			FILE *fmed = fopen(medFlowDir, "rb");
			FILE *fmix = fopen(mixFlowDir, "wb");

			ReadPcapFileHeader(fatk);
			ExtractPcapPacket(fatk, attackPacket);
			uint64_t attackStartUs = attackPacket->us;

			while (ExtractFormattedPacket(fmed, normalPacket))
			{	
				while (true)
				{
					if (US(attackPacket) >= normalPacket->us)
						break;

					attackPacket->us = US(attackPacket);
					attackPacket->id = ID(attackPacket);
					WriteFormattedPacket(fmix, attackPacket);

					if(attackList.insert(attackPacket->id).second)
					{
						++attackSeq;
						if (attackSeq == 4 || attackSeq == 5)
						{
							// WriteFormattedPacket(flist, attackPacket);
							++cnt;
						}
					}

					if (!ExtractPcapPacket(fatk, attackPacket))
					{
						fclose(fatk);
						fatk = fopen(attackFlowDir[k], "rb");
						ReadPcapFileHeader(fatk);
						ExtractPcapPacket(fatk, attackPacket);
						offset = normalPacket->us;
						continue;
					}
				}

				
				WriteFormattedPacket(fmix, normalPacket);
			}

			while (US(attackPacket) < datalen)
			{
				currentUs = US(attackPacket);
				attackPacket->us = US(attackPacket);
				attackPacket->id = ID(attackPacket);
				WriteFormattedPacket(fmix, attackPacket);

				if(attackList.insert(attackPacket->id).second)
				{
					++attackSeq;
					if (attackSeq == 4 || attackSeq == 5)
					{
						// WriteFormattedPacket(flist, attackPacket);
						++cnt;
					}
				}

				if (!ExtractPcapPacket(fatk, attackPacket))
				{
					fclose(fatk);
					fatk = fopen(attackFlowDir[k], "rb");
					ReadPcapFileHeader(fatk);
					ExtractPcapPacket(fatk, attackPacket);
					offset = currentUs + (uint64_t)((double)maxOffset * (double)rand() / (double)RAND_MAX);
					continue;
				}
			}

			fclose(fmix);
			fclose(fmed);
			fclose(fatk);
			// printf("\033[1A\033[KAttack flow %d inserted\n", i);

			fmed = fopen(medFlowDir, "wb");
			fmix = fopen(mixFlowDir, "rb");
			while (ExtractFormattedPacket(fmix, normalPacket))
				WriteFormattedPacket(fmed, normalPacket);
			fclose(fmix);
			fclose(fmed);
			
		}
	}

	// fclose(flist);
	printf("Done. Insert %d APT flows.\n", cnt);

	return 0;
}
