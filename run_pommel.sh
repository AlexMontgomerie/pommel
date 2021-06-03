# Paths to external programs
CACTI_PATH=/home/alex/cacti/cacti
DRAMPOWER_PATH=/home/alex/DRAMPower/drampower

# parameters
memory_config=$1
shift
featuremap_path=$1
shift
encoder=$1
shift
network_config=$1
shift
accelerator_config=$1
shift
output_path=$1
shift

# create output directory
mkdir -p $output_path

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
        -o ${output_path}/encoder_config.xml \
        -c $@

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
    ./DRAMPower/drampower -m $memory_config -c $partition/trace-chan-0-rank-0.cmdtrace  > $partition/dram_power.rpt

    # run cacti IO estimation
    cd cacti
        ./cacti -infile ../$partition/read_cacti.cfg  > ../$partition/read_cacti.rpt
        ./cacti -infile ../$partition/write_cacti.cfg > ../$partition/write_cacti.rpt
        ./cacti -infile ../$partition/idle_cacti.cfg  > ../$partition/idle_cacti.rpt
    cd ..

done

# generate report
python -m scripts.generate_report -p $output_path

# clean up
for partition in $output_path/*/ ; do
    # remove these files
    rm -r $partition
done

