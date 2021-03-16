# build tool TODO: remove later on
make clean
make -j 8

# parameters
#memory_config=config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml
#memory_config=config/memory/MICRON_4Gb_DDR4-2400_8bit_A.xml
#memory_config=config/memory/JEDEC_256Mb_WIDEIO_SDR-266_128bit.xml
memory_config=config/memory/ddr3.xml
#memory_config=config/memory/zedboard.xml
featuremap_path=data/alexnet.h5
accelerator_config=config/accelerator/tpu.xml
network_config=config/network/example.xml
output_path=outputs/tmp
encoder=baseline

mkdir -p $output_path
#rm -r $output_path/*

sh ./run_pommel.sh $memory_config $featuremap_path $encoder $network_config $accelerator_config $output_path

