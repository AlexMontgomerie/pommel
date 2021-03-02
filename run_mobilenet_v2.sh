#!/bin/bash

# build tool TODO: remove later on
make clean
make -j 8

ddr3_path=config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml

mobilenet_v2_network=config/network/mobilenet_v2.xml
mobilenet_v2_featuremap=data/mobilenet_v2.h5

tpu=config/accelerator/tpu.xml
eyeriss=config/accelerator/eyeriss.xml
scnn=config/accelerator/scnn.xml
shidiannao=config/accelerator/shidiannao.xml

run() {
    # parameters
    memory_config=$1
    featuremap_path=$2
    accelerator_config=$3
    network_config=$4
    output_path=$5
    encoder=$6

    mkdir -p $output_path
    rm -r $output_path/*

    sh ./run_pommel.sh $memory_config $featuremap_path $encoder $network_config $accelerator_config $output_path
}

memory_config=config/memory/MICRON_1Gb_DDR3-1600_8bit_G.xml
featuremap_path=data/alexnet.h5
accelerator_config=config/accelerator/tpu.xml
network_config=config/network/example.xml
output_path=outputs/tmp
encoder=rle


run $ddr3_path $mobilenet_v2_featuremap $tpu $mobilenet_v2_network outputs/tpu_mobilenet_v2_ddr3_baseline baseline
run $ddr3_path $mobilenet_v2_featuremap $tpu $mobilenet_v2_network outputs/tpu_mobilenet_v2_ddr3_bi bi
run $ddr3_path $mobilenet_v2_featuremap $tpu $mobilenet_v2_network outputs/tpu_mobilenet_v2_ddr3_rle rle
run $ddr3_path $mobilenet_v2_featuremap $tpu $mobilenet_v2_network outputs/tpu_mobilenet_v2_ddr3_def def
run $ddr3_path $mobilenet_v2_featuremap $tpu $mobilenet_v2_network outputs/tpu_mobilenet_v2_ddr3_huffman huffman 

run $ddr3_path $mobilenet_v2_featuremap $eyeriss $mobilenet_v2_network outputs/eyeriss_mobilenet_v2_ddr3_baseline baseline
run $ddr3_path $mobilenet_v2_featuremap $eyeriss $mobilenet_v2_network outputs/eyeriss_mobilenet_v2_ddr3_bi bi
run $ddr3_path $mobilenet_v2_featuremap $eyeriss $mobilenet_v2_network outputs/eyeriss_mobilenet_v2_ddr3_rle rle
run $ddr3_path $mobilenet_v2_featuremap $eyeriss $mobilenet_v2_network outputs/eyeriss_mobilenet_v2_ddr3_def def
run $ddr3_path $mobilenet_v2_featuremap $eyeriss $mobilenet_v2_network outputs/eyeriss_mobilenet_v2_ddr3_huffman huffman 

run $ddr3_path $mobilenet_v2_featuremap $scnn $mobilenet_v2_network outputs/scnn_mobilenet_v2_ddr3_baseline baseline
run $ddr3_path $mobilenet_v2_featuremap $scnn $mobilenet_v2_network outputs/scnn_mobilenet_v2_ddr3_bi bi
run $ddr3_path $mobilenet_v2_featuremap $scnn $mobilenet_v2_network outputs/scnn_mobilenet_v2_ddr3_rle rle
run $ddr3_path $mobilenet_v2_featuremap $scnn $mobilenet_v2_network outputs/scnn_mobilenet_v2_ddr3_def def
run $ddr3_path $mobilenet_v2_featuremap $scnn $mobilenet_v2_network outputs/scnn_mobilenet_v2_ddr3_huffman huffman

run $ddr3_path $mobilenet_v2_featuremap $shidiannao $mobilenet_v2_network outputs/shidiannao_mobilenet_v2_ddr3_baseline baseline
run $ddr3_path $mobilenet_v2_featuremap $shidiannao $mobilenet_v2_network outputs/shidiannao_mobilenet_v2_ddr3_bi bi
run $ddr3_path $mobilenet_v2_featuremap $shidiannao $mobilenet_v2_network outputs/shidiannao_mobilenet_v2_ddr3_rle rle
run $ddr3_path $mobilenet_v2_featuremap $shidiannao $mobilenet_v2_network outputs/shidiannao_mobilenet_v2_ddr3_def def
run $ddr3_path $mobilenet_v2_featuremap $shidiannao $mobilenet_v2_network outputs/shidiannao_mobilenet_v2_ddr3_huffman huffman


