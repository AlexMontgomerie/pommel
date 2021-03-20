# Paths to external programs
CACTI_PATH=/home/alex/cacti/cacti
DRAMPOWER_PATH=/home/alex/DRAMPower/drampower

# parameters
memory_config=$1
featuremap_path=$2
encoder=$3
network_config=$4
accelerator_config=$5
output_path=$6

if [ "$encoder" = "baseline" ]; then

    # run silence
    ./bin/main \
        --baseline \
        --memory $memory_config \
        --featuremap $featuremap_path \
        --encoder $encoder \
        --network $network_config \
        --accelerator $accelerator_config \
        --output $output_path

else

    # generate encoder config
    python scripts/generate_encoder_config.py -e $encoder \
        -a $accelerator_config \
        -n $network_config \
        -f $featuremap_path \
        -o ${output_path}/encoder_config.xml

    encoder_config=${output_path}/encoder_config.xml

    # run silence
    ./bin/main \
        --memory $memory_config \
        --featuremap $featuremap_path \
        --encoder $encoder_config \
        --network $network_config \
        --accelerator $accelerator_config \
        --output $output_path 

fi

# iterate over folders in output directory
for partition in $output_path/*/ ; do
    
    # run DRAM Power estimation
    ./DRAMPower/drampower -m $memory_config -c $partition/input-chan-0-rank-0.cmdtrace  > $partition/input_dram_power.rpt
    ./DRAMPower/drampower -m $memory_config -c $partition/output-chan-0-rank-0.cmdtrace > $partition/output_dram_power.rpt

    # run cacti IO estimation 
    cd cacti
        ./cacti -infile ../$partition/input_cacti.cfg  > ../$partition/input_cacti.rpt
        ./cacti -infile ../$partition/output_cacti.cfg > ../$partition/output_cacti.rpt
    cd ..

done

# generate report
python scripts/add_power_to_report.py -p $output_path

# clean up
for partition in $output_path/*/ ; do
    # remove these files
    rm -r $partition
done

