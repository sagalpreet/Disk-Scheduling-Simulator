struct disk
{
	int R; // roatational speed of the disk in rpm
	int T; // average seek time
	int N; // sector size in bytes

	int __curr_sector;
	int __curr_track;

	char __head_direction; // required for scan algo: 1=cw & -1=acw
};

struct address
{
	int platter, cylinder, sector;
};


void init(struct disk *d);
int seek(struct disk *d, struct address a);
