#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "disk.h"
#include "request.h"
#include "scheduler.h"

const int DEBUG = 0;

struct disk DISK;

/*
Further assumptions:
1) The disk contains 4 surfaces and 4 read-write heads
2) 25 cylinders
3) 20 sectors per track
*/
int SURFACES = 4;
int CYLINDERS = 25;
int SECTORS = 20;

policy POLICY = RANDOM;
int NUM_REQUESTS = 1000;
int TIME_GAP = 2; // choose this wisely else all your policies might end up being fifo

int main(int argc, char **argv)
{
	FILE *f = fopen("../results/data.txt", "a");
	srand(time(0));
	argc--;
	if (argc != 4)
	{
		printf("Error: exactly 4 command line arguments need to be given\n");
		exit(0);
	}

	DISK.R = atoi(argv[1]);
	DISK.N = atoi(argv[2]);
	DISK.T = atoi(argv[3]);
	POLICY = atoi(argv[4]);
	init(&DISK);

	printf("Initial position of head: %d\n", DISK.__curr_track);
	printf("Initial position of platter: %d\n", DISK.__curr_sector);

	// get request queue
	struct results stats = schedule();
	
	char policy[7];
	switch(POLICY)
	{
		case RANDOM: strcpy(policy, "Random\0"); break;
		case FIFO: strcpy(policy, "FIFO  \0"); break;
		case SSTF: strcpy(policy, "SSTF  \0"); break;
		case SCAN: strcpy(policy, "SCAN  \0"); break;
		case CSCAN: strcpy(policy, "CSCAN \0"); break;
		default: printf("Wrong Policy Arguement\n");
	}

	fprintf(f, "    %06d      %06d      %06d        %s       %06lld       %06lld        %013.6lf          %013.6lf       %06lld   ",
	DISK.R, DISK.T, DISK.N, policy, stats.min_response_time,
	stats.max_response_time, stats.average_response_time,
	stats.std_dev_response_time, stats.throughput);
	fprintf(f, "\n");

	fclose(f);
	return 0;
}
