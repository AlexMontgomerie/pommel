# build tool TODO: remove later on
make clean
make -j 8

# parameters
memory_config=config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml
memory_config=config/memory/MICRON_4Gb_LPDDR3-1600_32bit_A.xml
#memory_config=config/memory/JEDEC_256Mb_WIDEIO_SDR-266_128bit.xml
featuremap_path=data/activity.h5
encoder_config=test.xml
accelerator_config=config/accelerator/bandwidth_activity_test_ddr3.xml
accelerator_config=config/accelerator/bandwidth_activity_test_lpddr.xml
#accelerator_config=config/accelerator/bandwidth_activity_test_wideio.xml
output_path=outputs/activity_bandwidth_ddr3
output_path=outputs/activity_bandwidth_lpddr
#output_path=outputs/activity_bandwidth_wideio

mkdir -p $output_path
rm -r $output_path/*

sh ./run_pommel.sh $memory_config $featuremap_path $encoder_config $accelerator_config $output_path

python scripts/add_power_to_report.py -p $output_path
