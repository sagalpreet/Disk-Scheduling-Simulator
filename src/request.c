#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "disk.h"
#include "request.h"

extern struct disk DISK;
extern int SURFACES, CYLINDERS, SECTORS;

struct request* generate_requests(int num_requests, int time_gap)
{
    struct request *request = malloc (num_requests * sizeof(struct request));
    long long time = 0;

    for (int i = 0; i < num_requests; i++)
    {
        request[i].num_sectors = rand() % SECTORS + 1;
        request[i].address.cylinder = rand() % CYLINDERS;
        request[i].address.platter = rand() % SURFACES;
        request[i].address.sector = rand() % SECTORS;

        request[i].spawn_time = time;
        time += time_gap;
    }

    FILE* f = fopen("../results/requests.txt", "a");
    for (int i = 0; i < num_requests; i++)
    {
        fprintf(f, "<%lld: %d, %d, %d>    ", request[i].spawn_time, request[i].address.platter, request[i].address.cylinder, request[i].address.sector);
    }
    fprintf(f, "\n");
    fclose(f);

    return request;
}