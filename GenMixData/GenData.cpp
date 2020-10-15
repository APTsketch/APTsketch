#include <iostream>
#include <random>
#include <set>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <typeinfo>
#include <memory.h>
#include "parser.h"

#define LSTRING 100
#define INSERTNUM 1
#define US(attackPacket) ((uint64_t)((double)(attackPacket->us)))
#define ID(attackPacket) (attackPacket->id)
using namespace std;

const char comment[LSTRING] = "sim2.7_2to4_100";

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Wrong Input!\n");
		return 0;
	}

	/* Background Flows */	
	char normalFlowDir[LSTRING] = "../data/normal/simbg_2.700000_1000_300000_500_10000000.dat";
	FILE *fnrm = fopen(normalFlowDir, "rb");

	/* Attack Flows */
	char attackFlowDir[INSERTNUM][LSTRING] = {
		"../data/mix/atk_2to4_100.dat"
	};

	uint64_t sparsifyLevel = atoi(argv[1]);
	srand((unsigned)time(NULL));
	
	/* Median File */
	char medFlowDir[LSTRING] = "../data/mix/intermediate.dat";
	FILE *fmed = fopen(medFlowDir, "wb");

	/* Output files */
	char mixFlowDir[LSTRING] = "";
	sprintf(mixFlowDir, "../data/mix/%s.dat", comment);

	Packet *normalPacket = new Packet;
	Packet *attackPacket = new Packet;
	printf("%s\n", comment);

	printf("begin to copy...\n");
	ExtractFormattedPacket(fnrm, normalPacket);
	uint64_t normalStartUs = normalPacket->us;
	normalPacket->us = 0;
	
	WriteFormattedPacket(fmed, normalPacket);
	while (true) 
	{
		if (!ExtractFormattedPacket(fnrm, normalPacket))
			break;
		normalPacket->us = (normalPacket->us - normalStartUs) * sparsifyLevel;
		WriteFormattedPacket(fmed, normalPacket);
	}

	fclose(fmed);
	fclose(fnrm);

	FILE *fatk = fopen(attackFlowDir[0], "rb");
	fmed = fopen(medFlowDir, "rb");
	FILE *fmix = fopen(mixFlowDir, "wb");
	ExtractFormattedPacket(fatk, attackPacket);

	bool atk = true;
	while (ExtractFormattedPacket(fmed, normalPacket))
	{
		while (atk)
		{
			if (attackPacket->us >= normalPacket->us)
				break;
			WriteFormattedPacket(fmix, attackPacket);

			if (!ExtractFormattedPacket(fatk, attackPacket))
			{
				atk = false;
			}
		}
		WriteFormattedPacket(fmix, normalPacket);
	}
	fclose(fmed);
	fclose(fatk);
	fclose(fmix);
	return 0;
}
