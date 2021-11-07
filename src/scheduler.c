#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "disk.h"
#include "request.h"
#include "scheduler.h"

#define INF 1e9
#define min(X, Y) (((X) < (Y)) ? (X) : (Y))
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))

extern const int DEBUG;

extern struct disk DISK;

extern int SURFACES;
extern int CYLINDERS;
extern int SECTORS;

int NUM_REQUESTS;
int TIME_GAP;
extern policy POLICY;

struct results schedule()
{
    /*
    returns the statistics related to scheduling
    namely; average, min, max and std-dev of response times and throughput
    */

    struct request *request = generate_requests(NUM_REQUESTS, TIME_GAP);
    long long response_time[NUM_REQUESTS];

    for (int i = 0; i < NUM_REQUESTS; i++) response_time[i] = -1;

    long long time = 0;
    for (int i = 0; i < NUM_REQUESTS; i++)
    {
        int index = service(request, response_time, &time);
		time += seek(&DISK, request[index].address);
		response_time[index] = time - request[index].spawn_time;
    }

	if (DEBUG) for (int i = 0; i < NUM_REQUESTS; i++) printf("%lld ", response_time[i]);
	
	struct results results;
	results.throughput = (1000 * NUM_REQUESTS) / time;
	get_stats(response_time, &results);

	free(request);
	return results;
}

int service(struct request *request, long long *response_time, long long *time)
{
	long long earliest_next_time = INF;
	for (int i = 0; i < NUM_REQUESTS; i++)
		if (response_time[i] == -1)
			earliest_next_time = min(earliest_next_time, request[i].spawn_time);

	*time = max(*time, earliest_next_time);

    switch(POLICY)
    {
        case RANDOM: return _random(request, response_time, time); break;
        case FIFO: return _fifo(request, response_time, time); break;
        case SSTF: return _sstf(request, response_time, time); break;
        case SCAN: return _scan(request, response_time, time); break;
        case CSCAN: return _cscan(request, response_time, time); break;
        default: printf("ERROR: no such policy has been defined\n");
    } 
}

int _random(struct request *request, long long *response_time, long long *time)
{
	int rand_index = rand() % NUM_REQUESTS;
	for (int i = 0; i < NUM_REQUESTS; i++)
	{
		int index = (rand_index + i) % NUM_REQUESTS;
		if (response_time[index] == -1 && request[index].spawn_time <= *time) return index;
	}
}

int _fifo(struct request *request, long long *response_time, long long *time)
{
	int first_come = -1;
	long long earliest_time = INF;
	for (int i = 0; i < NUM_REQUESTS; i++)
	{
		if (response_time[i] == -1 && request[i].spawn_time <= *time)
		{
			if (request[i].spawn_time < earliest_time)
			{
				earliest_time = request[i].spawn_time;
				first_come = i;
			}
		}
	}
	*time = earliest_time;
	return first_come;
}

int _sstf(struct request *request, long long *response_time, long long *time)
{
	int closest_index = -1, closest_value = CYLINDERS + 1;
	for (int i = 0; i < NUM_REQUESTS; i++)
	{
		if (response_time[i] == -1 && request[i].spawn_time <= *time)
		{
			if (abs(DISK.__curr_track - (request[i].address).cylinder) < closest_value)
			{
				closest_index = i;
				closest_value = abs(DISK.__curr_track - (request[i].address).cylinder);
			}
		}
	}
	return closest_index;
}

int _scan(struct request *request, long long *response_time, long long *time)
{
    int closest_index = -1, closest_value = CYLINDERS + 1;

	for (int i = 0; i < NUM_REQUESTS; i++)
	{
		if (response_time[i] == -1 && request[i].spawn_time <= *time)
		{
			int dis = abs(DISK.__curr_track - (request[i].address).cylinder);
			int gap = DISK.__curr_track - (request[i].address).cylinder;

			if (dis < closest_value && gap == dis * DISK.__head_direction)
			{
				closest_index = i;
				closest_value = dis;
			}
		}
	}

	if (closest_index != -1) return closest_index;

	// opposite direction scan
	for (int i = 0; i < NUM_REQUESTS; i++)
	{
		if (response_time[i] == -1 && request[i].spawn_time <= *time)
		{
			int dis = abs(DISK.__curr_track - (request[i].address).cylinder);
			int gap = DISK.__curr_track - (request[i].address).cylinder;

			if (dis < closest_value && gap * (-1) == dis * DISK.__head_direction)
			{
				closest_index = i;
				closest_value = dis;
			}
		}
	}

	if (closest_index != -1)
	{
		DISK.__head_direction *= -1;
		return closest_index;
	}
}

int _cscan(struct request *request, long long *response_time, long long *time)
{
    int closest_index = -1, closest_value = 999999999;

	// current direction scan
	for (int i = 0; i < NUM_REQUESTS; i++)
	{
		if (response_time[i] == -1 && request[i].spawn_time <= *time)
		{
			int dis = abs(DISK.__curr_track - (request[i].address).cylinder);
			int gap = (request[i].address).cylinder - DISK.__curr_track;

			if (dis < closest_value && gap == dis)
			{
				closest_index = i;
				closest_value = dis;
			}
		}
	}

	if (closest_index != -1) return closest_index;

	// opposite direction scan
	struct address home = {-1, 0, -1}; // -1 implies doesn't matter
	(*time) += seek(&DISK, home);

	for (int i = 0; i < NUM_REQUESTS; i++)
	{
		if (response_time[i] == -1 && request[i].spawn_time <= *time)
		{
			int dis = abs(DISK.__curr_track - (request[i].address).cylinder);
			int gap = (request[i].address).cylinder - DISK.__curr_track;

			if (dis < closest_value && gap == dis)
			{
				closest_index = i;
				closest_value = dis;
			}
		}
	}

	return closest_index;	
}

void get_stats(long long *ar, struct results *results)
{
	long long sm = 0;
	for (int i = 0; i < NUM_REQUESTS; i++) sm += ar[i];

	(*results).average_response_time = ((double) sm) / NUM_REQUESTS;
	(*results).max_response_time = (*results).std_dev_response_time = 0;
	(*results).min_response_time = 999999999;

	for (int i = 0; i < NUM_REQUESTS; i++)
	{
		(*results).max_response_time = max((*results).max_response_time, ar[i]);
		(*results).min_response_time = min((*results).min_response_time, ar[i]);
		(*results).std_dev_response_time += pow(ar[i] - (*results).average_response_time, 2);
	}

	(*results).std_dev_response_time /= NUM_REQUESTS;
	(*results).std_dev_response_time = pow((*results).std_dev_response_time, 0.5);
}
