#!/bin/bash

# build tool TODO: remove later on
make clean
make -j 8

ddr3_path=config/memory/MICRON_2GB_DDR3-1600_64bit_G_UDIMM.xml
ddr4_path=config/memory/MICRON_4Gb_DDR4-2400_8bit_A.xml
wide_io_path=config/memory/JEDEC_256Mb_WIDEIO_SDR-266_128bit.xml
zedboard_path=config/memory/zedboard.xml

alexnet_network=config/network/alexnet.xml
alexnet_featuremap=data/alexnet.h5

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

# DDR3
run $ddr3_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr3_baseline baseline
run $ddr3_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr3_bi bi
run $ddr3_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr3_rle rle
run $ddr3_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr3_def def
run $ddr3_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr3_pbm pbm 
run $ddr3_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr3_huffman huffman 

run $ddr3_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr3_baseline baseline
run $ddr3_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr3_bi bi
run $ddr3_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr3_rle rle
run $ddr3_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr3_def def
run $ddr3_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr3_pbm pbm
run $ddr3_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr3_huffman huffman 

run $ddr3_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr3_baseline baseline
run $ddr3_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr3_bi bi
run $ddr3_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr3_rle rle
run $ddr3_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr3_def def
run $ddr3_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr3_pbm pbm 
run $ddr3_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr3_huffman huffman

run $ddr3_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr3_baseline baseline
run $ddr3_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr3_bi bi
run $ddr3_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr3_rle rle
run $ddr3_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr3_def def
run $ddr3_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr3_pbm pbm
run $ddr3_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr3_huffman huffman

# DDR4

run $ddr4_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr4_baseline baseline
run $ddr4_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr4_bi bi
run $ddr4_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr4_rle rle
run $ddr4_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr4_def def
run $ddr4_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr4_pbm pbm
run $ddr4_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_ddr4_huffman huffman 

run $ddr4_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr4_baseline baseline
run $ddr4_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr4_bi bi
run $ddr4_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr4_rle rle
run $ddr4_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr4_def def
run $ddr4_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr4_pbm pbm
run $ddr4_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_ddr4_huffman huffman 

run $ddr4_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr4_baseline baseline
run $ddr4_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr4_bi bi
run $ddr4_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr4_rle rle
run $ddr4_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr4_def def
run $ddr4_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr4_pbm pbm
run $ddr4_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_ddr4_huffman huffman

run $ddr4_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr4_baseline baseline
run $ddr4_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr4_bi bi
run $ddr4_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr4_rle rle
run $ddr4_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr4_def def
run $ddr4_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr4_pbm pbm
run $ddr4_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_ddr4_huffman huffman

# WIDE-IO

run $wide_io_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_wide_io_baseline baseline
run $wide_io_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_wide_io_bi bi
run $wide_io_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_wide_io_rle rle
run $wide_io_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_wide_io_def def
run $wide_io_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_wide_io_pbm pbm
run $wide_io_path $alexnet_featuremap $tpu $alexnet_network outputs/tpu_alexnet_wide_io_huffman huffman 
run $wide_io_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_wide_io_baseline baseline
run $wide_io_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_wide_io_bi bi
run $wide_io_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_wide_io_rle rle
run $wide_io_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_wide_io_def def
run $wide_io_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_wide_io_pbm pbm
run $wide_io_path $alexnet_featuremap $eyeriss $alexnet_network outputs/eyeriss_alexnet_wide_io_huffman huffman 
run $wide_io_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_wide_io_baseline baseline
run $wide_io_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_wide_io_bi bi
run $wide_io_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_wide_io_rle rle
run $wide_io_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_wide_io_def def
run $wide_io_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_wide_io_pbm pbm
run $wide_io_path $alexnet_featuremap $scnn $alexnet_network outputs/scnn_alexnet_wide_io_huffman huffman
run $wide_io_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_wide_io_baseline baseline
run $wide_io_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_wide_io_bi bi
run $wide_io_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_wide_io_rle rle
run $wide_io_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_wide_io_def def
run $wide_io_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_wide_io_pbm pbm
run $wide_io_path $alexnet_featuremap $shidiannao $alexnet_network outputs/shidiannao_alexnet_wide_io_huffman huffman


