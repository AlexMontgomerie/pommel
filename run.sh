make clean
make -j 8

./bin/main -e def -i data/test.dat -o data/test_out.dat 
./bin/main -e abe -i data/test.dat -o data/test_out.dat 
./bin/main -e bi  -i data/test.dat -o data/test_out.dat 
./bin/main -e awr -i data/test.dat -o data/test_out.dat 
./bin/main -e rle -i data/test.dat -o data/test_out.dat 
