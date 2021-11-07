Submitter name: Sagalpreet Singh
Roll No.: 2019csb1113
Course: CS303
=================================
1. What does this program do

This program simulates the process of scheduling requests for disk access using Random, FIFO, SSTF, SCAN and CSCAN algorithms. Statistics like mean and standard deviation of response time for these algorithms is stored and analyzed and performance of these algorithms is discussed. 
Program is run with different input parameters:

1. Rotational speed of disk, r revolutions per minute.
2. Average seek time, T in ms.
3. Sector size, N in bytes.
4. Disk Scheduling Algorithm (by its code).

These parameters are taken as command line arguments.

Parameters to gauge performance:

1. Minimum Response Time
2. Maximum Response Time
3. Average Response Time
4. Standard Deviation of Response Time
5. Throughput

FILE STRUCTURES
.
├── bin
│   └── run
├── LICENSE
├── obj
│   ├── disk.o
│   ├── main.o
│   ├── request.o
│   └── scheduler.o
├── README.txt
├── results
│   ├── data.txt
│   └── requests.txt
├── scripts
│   ├── generate_result.sh
│   └── run.sh
├── src
│   ├── disk.c
│   ├── disk.h
│   ├── main
│   ├── main.c
│   ├── request.c
│   ├── request.h
│   ├── scheduler.c
│   └── scheduler.h
└── test

Assumption: Disk Scheduling Algorithms themselves take no time.

=================================
2. A description of how this program works (i.e. its logic)

- Address : Is defined by Platter, Sector and Track on the hard drive
- Request Generation : Requests are generated before hand with the following attributes for each request: Spawn Time, Address, Number of Sectors. All the requests are uniformly generated with constant time gap. This time gap is specified as hyperparameter in the main.c file. The attributes for each request are randomly generated.
- The disk scheduling algorithms are implemented as follows:
    - A global time variable is maintained (initialized to 0)
    - It checks from all the pending requests (by comparing current time and spawn time of each request) to find the suitable one according to the algorithm. Processes it and increments the time accordingly.
    - Before the disk scheduler runs, it is checked if there is a pending request. If there is no such request, time is advanced to move to the nearest next time when a request spawns.
- The number of requests is also a hyperparameter set in main.c
- The hard disk stores the current location of head (which sector and track it lies on). Only one such instance is maintained as it will be same for all the platters (this is how most hard disks work nowadays). In addition to this, the direction in which head is currently moving is also stored.
- Proper care has been taken to rotate the disk at the time when head is moving. (Disks keep on rotating and don't stop unless powered off). Thus it may happen that although disk was in right position but head was not on proper track and in time when head came to proper track, the disk had rotated from its original direction, so more time will be taken now.
- Two things are stored in the results directory:
    - data.txt : It contains the statistics
    - requests.txt : It contains the request queue that was generated, one in each line (without word wrap of course)
    * The data.txt file may not be nice formatted if run directly without bash script as instructed in further section.
    
- Default Hyperparameters
    - SURFACES = 4
    - CYLINDERS = 25
    - SECTORS = 20
    - NUM_REQUESTS = 1000
    - TIME_GAP = 5
    
* ms is used as unit of time everywhere except throughput for which it is "number of requests per second"
    
=================================
3. How to compile and run this program

- To generate results (should take around 3-4 seconds for default settings)
	From inside scripts directory, execute following command:
	> ./generate_result.sh
	or
	> sh generate_result.sh

	This generates data.txt and requests.txt files in results directory (I have run on my machine and generated the file)
	Every time this is run, previous files are erased.

- To test on some parameters:
	From inside scripts directory, execute following command:
	> ./run.sh 7500 512 4 0
	or
	> sh run.sh 7500 512 4 0
	
	Parameters:
	1st => Disk Rotational Speed
	2nd => Sector size in bytes
	3rd => Average Seek Time (in ms)
	4th => Algorithm (0: Random, 1: FIFO, 2: SSTF, 3: SCAN, 4: CSCAN)
	
=================================
4. Provide a snapshot of a sample run

- The data.txt and requests.txt can be viewed from inside results directory.

RESULTS:

|      R     |     T     |    N    |     POLICY     |  MIN-RT |    MAX-RT    |     AVERAGE-RT      |     STD-DEV-RT      | THROUGHPUT|
    007500      000004      000512        Random       000003       003142        001516.028000          000977.539316       000123   
    015000      000004      000512        Random       000004       000520        000244.786000          000131.727682       000181   
    007500      000004      000512        FIFO         000000       000020        000008.074000          000004.156263       000200   
    015000      000004      000512        FIFO         000000       000014        000005.458000          000003.152814       000200   
    007500      000004      000512        SSTF         000001       000189        000048.860000          000038.358081       000197   
    015000      000004      000512        SSTF         000000       000050        000009.372000          000006.086840       000199   
    007500      000004      000512        SCAN         000001       000152        000041.716000          000029.414951       000198   
    015000      000004      000512        SCAN         000000       000032        000009.723000          000005.710540       000199   
    007500      000004      000512        CSCAN        000001       000226        000068.810000          000043.519259       000199   
    015000      000004      000512        CSCAN        000000       000042        000016.082000          000008.669676       000199   
    
* RT stands for response time.
* Ignore leading zeros, they are just for indentation purposes

The inferences and analysis can be found in design.pdf
