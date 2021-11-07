typedef enum policy{RANDOM, FIFO, SSTF, SCAN, CSCAN} policy;

struct results
{
    long long min_response_time, max_response_time, throughput;
	double average_response_time, std_dev_response_time;
};

struct results schedule();
int service(struct request *request, long long *response_time, long long *time);
int _random(struct request *request, long long *response_time, long long *time);
int _fifo(struct request *request, long long *response_time, long long *time);
int _sstf(struct request *request, long long *response_time, long long *time);
int _scan(struct request *request, long long *response_time, long long *time);
int _cscan(struct request *request, long long *response_time, long long *time);
void get_stats(long long *ar, struct results *results);