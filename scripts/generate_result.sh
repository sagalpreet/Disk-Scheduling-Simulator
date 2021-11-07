rm ../results/*

echo -e "|      R     |     T     |    N    |     POLICY     |  MIN-RT |    MAX-RT    |     AVERAGE-RT      |     STD-DEV-RT      | THROUGHPUT|" >> ../results/data.txt

./run.sh 7500 512 4 0
./run.sh 15000 512 4 0
./run.sh 7500 512 4 1
./run.sh 15000 512 4 1
./run.sh 7500 512 4 2
./run.sh 15000 512 4 2
./run.sh 7500 512 4 3
./run.sh 15000 512 4 3
./run.sh 7500 512 4 4
./run.sh 15000 512 4 4