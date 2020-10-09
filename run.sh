VAMPIRE_PATH=/home/alex/VAMPIRE
RAMULATOR_PATH=/home/alex/ramulator
CACTI_PATH=/home/alex/cacti

make clean
make -j 8

./bin/main -e def -i data/test.dat -o data/test_out.dat -t data/test.trace
# run ramulator trace
$RAMULATOR_PATH/ramulator $RAMULATOR_PATH/configs/DDR3-config.cfg --mode=dram data/test.trace

# run vampire power estimation

# run cacti estimation


#./bin/main -e abe -i data/test.dat -o data/test_out.dat 
#./bin/main -e bi  -i data/test.dat -o data/test_out.dat 
#./bin/main -e awr -i data/test.dat -o data/test_out.dat 
#./bin/main -e rle -i data/test.dat -o data/test_out.dat 
