# Paths to external programs
CACTI_PATH=/home/alex/cacti/cacti
DRAMPOWER_PATH=/home/alex/DRAMPower/drampower

# parameters
memory_config=$1
featuremap_path=$2
encoder_config=$3
accelerator_config=$4
output_path=$5

# run silence
./bin/main \
    --baseline \
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

