# build tool TODO: remove later on
make clean
make -j 8

# parameters
#memory_config=config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml
memory_config=config/memory/ddr4.xml
#memory_config=config/memory/JEDEC_256Mb_WIDEIO_SDR-266_128bit.xml
#memory_config=config/memory/wide_io_single.xml
#memory_config=config/memory/zedboard.xml
featuremap_path=featuremaps/alexnet_8b.h5
accelerator_config=config/accelerator/tpu.xml
network_config=config/network/alexnet_tpu.xml
output_path=outputs/tmp
encoder=bi

mkdir -p $output_path
#rm -r $output_path/*

sh ./run_pommel.sh $memory_config $featuremap_path $encoder $network_config $accelerator_config $output_path

