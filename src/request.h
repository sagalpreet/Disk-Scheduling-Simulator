struct request
{
    int num_sectors;
    long long spawn_time;
    struct address address;
};

struct request* generate_requests();