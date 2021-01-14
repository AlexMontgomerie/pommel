# Paths to external programs
CACTI_PATH=/home/alex/cacti/cacti
DRAMPOWER_PATH=/home/alex/DRAMPower/drampower

# build tool TODO: remove later on
make clean
make -j 8

# parameters
memory_config=config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml
#memory_config=config/memory/JEDEC_256Mb_WIDEIO_SDR-266_128bit.xml
featuremap_path=data/test.h5
encoder_config=test.xml
accelerator_config=config/accelerator/fpgaconvnet.xml
output_path=outputs/test

# run silence
./bin/main \
    --memory $memory_config \
    --featuremap $featuremap_path \
    --encoder $encoder_config \
    --accelerator $accelerator_config \
    --output $output_path

# iterate over folders in output directory
for partition in $output_path/*/ ; do
    
    # run DRAM Power estimation
    $DRAMPOWER_PATH -m $memory_config -c $partition/input-chan-0-rank-0.cmdtrace  > $partition/input_dram_power.rpt
    $DRAMPOWER_PATH -m $memory_config -c $partition/output-chan-0-rank-0.cmdtrace > $partition/output_dram_power.rpt

    # run cacti IO estimation 
    cd cacti
        ./cacti -infile ../$partition/input_cacti.cfg  > ../$partition/input_cacti.rpt
        ./cacti -infile ../$partition/output_cacti.cfg > ../$partition/output_cacti.rpt
    cd ..

done

