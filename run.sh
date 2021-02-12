# build tool TODO: remove later on
make clean
make -j 8

# parameters
memory_config=config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml
#memory_config=config/memory/MICRON_4Gb_LPDDR3-1600_32bit_A.xml
#memory_config=config/memory/JEDEC_256Mb_WIDEIO_SDR-266_128bit.xml
featuremap_path=data/mobilenet_v2.h5
encoder_config=config/encoder/baseline.xml
#accelerator_config=config/accelerator/eyeriss_alexnet.xml
accelerator_config=config/accelerator/eyeriss.xml
network_config=config/network/mobilenet_v2.xml
output_path=outputs/eyeriss_mobilenet_v2_ddr3

mkdir -p $output_path
rm -r $output_path/*

sh ./run_pommel.sh $memory_config $featuremap_path $encoder_config $network_config $accelerator_config $output_path

python scripts/add_power_to_report.py -p $output_path
