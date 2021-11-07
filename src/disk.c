#include <stdio.h>
#include <stdlib.h>
#include "disk.h"

extern int SURFACES;
extern int CYLINDERS;
extern int SECTORS;

void init(struct disk *d)
{
	d -> __curr_sector = rand() % SECTORS;
	d -> __curr_track = rand() % CYLINDERS;
	d -> __head_direction = 1;
}

int seek(struct disk *d, struct address a)
{
	int time_elapsed = 0; // in milliseconds

	// take care of -1 cases

	// moving head to required position and updating sector as well
	if (a.cylinder != -1)
	{
		long long time = abs(d->__curr_track - a.cylinder);
		time = ((3 * d->T * time) / CYLINDERS); // because T is average seek time
		d->__curr_track = a.cylinder;
		
		long long sectors_per_ms = (d->R * SECTORS) / 60000;
		d->__curr_sector += (time * sectors_per_ms) % SECTORS;
		d->__curr_sector %= SECTORS;

		time_elapsed += time;
	}

	// catching on the right sector
	if (a.sector != -1)
	{
		long long time = (a.sector - d->__curr_sector);
		if (time < 0) time += SECTORS;

		long long sectors_per_ms = (d->R * 60000) / SECTORS;
		time = time / sectors_per_ms;
		d->__curr_sector = a.sector;

		time_elapsed += time;
	}

	return time_elapsed;
}