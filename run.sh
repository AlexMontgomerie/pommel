# build tool TODO: remove later on
make clean
make -j 8

# parameters
memory_config=config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml
featuremap_path=data/alexnet.h5
accelerator_config=config/accelerator/example.xml
network_config=config/network/example.xml
output_path=outputs/eyeriss_mobilenet_v2_ddr3
encoder=rle

mkdir -p $output_path
rm -r $output_path/*

sh ./run_pommel.sh $memory_config $featuremap_path $encoder $network_config $accelerator_config $output_path

python scripts/add_power_to_report.py -p $output_path
