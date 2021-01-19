# build tool TODO: remove later on
make clean
make -j 8

# parameters
memory_config=config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml
featuremap_path=data/activity.h5
encoder_config=test.xml
accelerator_config=config/accelerator/bandwidth_activity_test.xml
output_path=outputs/activity_bandwidth

sh ./run_pommel.sh $memory_config $featuremap_path $encoder_config $accelerator_config $output_path
