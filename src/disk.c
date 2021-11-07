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

double seek(struct disk *d, struct address a)
{
	double time_elapsed = 0; // in milliseconds

	// take care of -1 cases

	// moving head to required position and updating sector as well
	if (a.cylinder != -1)
	{
		double time = abs(d->__curr_track - a.cylinder);
		time = ((3 * d->T * time) / CYLINDERS); // because T is average seek time
		d->__curr_track = a.cylinder;
		
		double sectors_per_ms = (d->R * SECTORS) / 60000;
		d->__curr_sector += ((int) (time * sectors_per_ms)) % SECTORS;
		d->__curr_sector %= SECTORS;

		time_elapsed += time;
	}

	// catching on the right sector
	if (a.sector != -1)
	{
		double time = (a.sector - d->__curr_sector);
		if (time < 0) time += SECTORS;

		double sectors_per_ms = (d->R * SECTORS) / 60000;
		if (sectors_per_ms != 0) time = time / sectors_per_ms;
		else printf("garbage results: very low head speed\n");
		d->__curr_sector = a.sector;

		time_elapsed += time;
	}

	return time_elapsed;
}