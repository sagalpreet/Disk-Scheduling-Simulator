#echo -e "\nClearing Previous Files...\n"

#echo -e "\nCompiling Begins...\n"

gcc -c ../src/disk.c -o ../obj/disk.o
gcc -c ../src/main.c -o ../obj/main.o
gcc -c ../src/request.c -o ../obj/request.o
gcc -c ../src/scheduler.c -o ../obj/scheduler.o -lm

#echo -e "\nLinking Begins...\n"
gcc ../obj/disk.o ../obj/main.o ../obj/request.o ../obj/scheduler.o -o ../bin/run -lm

#echo -e "\nExecution Begins...\n"
#echo -e "|      R     |     T     |    N    |     POLICY     |  MIN-RT |    MAX-RT    |     AVERAGE-RT      |     STD-DEV-RT      | THROUGHPUT|" >> ../results/data.txt
$run ../bin/run $1 $2 $3 $4