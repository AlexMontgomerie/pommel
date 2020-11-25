# Paths to external programs
RAMULATOR_PATH=/home/alex/ramulator/ramulator
CACTI_PATH=/home/alex/cacti/cacti
SCALE_SIM_PATH=/home/alex/SCALE-Sim
DRAMPOWER_PATH=/home/alex/DRAMPower/drampower

# build tool TODO: remove later on
make clean
make -j 8

./bin/main -e def #-i data/test.dat -o data/test_out.dat -t data/test.trace 
#./bin/main -e def --scale-sim-trace dram_filter.csv #-i data/test.dat -o data/test_out.dat -t data/test.trace 
#./bin/main -e def -i data/test.dat -o data/test_out.dat -t data/test.trace 
# run ramulator trace
#$RAMULATOR_PATH/ramulator $RAMULATOR_PATH/configs/DDR3-config.cfg --mode=dram tmp.trace

# run vampire power estimation
#$DRAMPOWER_PATH -m config/memory/MICRON_1Gb_DDR3-1066_16bit_G_2s.xml -c cmd-trace-chan-0-rank-0.cmdtrace 
# run cacti estimation
#$CACTI_PATH

#./bin/main -e abe -i data/test.dat -o data/test_out.dat 
#./bin/main -e bi  -i data/test.dat -o data/test_out.dat 
#./bin/main -e awr -i data/test.dat -o data/test_out.dat 
#./bin/main -e rle -i data/test.dat -o data/test_out.dat 
