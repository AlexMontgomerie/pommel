# Paths to external programs
CACTI_PATH=/home/alex/cacti/cacti
DRAMPOWER_PATH=/home/alex/DRAMPower/drampower

# build tool TODO: remove later on
make clean
make -j 8

# run silence
./bin/main \
    --memory config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml \
    --featuremap data/test.h5 \
    --encoder test.xml \
    --accelerator config/accelerator/fpgaconvnet.xml \
    --output outputs/test

# run vampire power estimation
#$DRAMPOWER_PATH -m config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml -c outputs/test/0/input-chan-0-rank-0.cmdtrace
#$DRAMPOWER_PATH -m config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml -c outputs/test/0/output-chan-0-rank-0.cmdtrace

# run cacti estimation
$CACTI_PATH -infile outputs/test/0/input_cacti.cfg 
#$CACTI_PATH -infile outputs/test/0/output_cacti.cfg 
