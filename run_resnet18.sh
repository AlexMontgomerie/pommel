#!/bin/bash

# build tool TODO: remove later on
make clean
make -j 8

ddr3_path=config/memory/MICRON_2GB_DDR3-1600_64bit_G_UDIMM.xml
ddr4_path=config/memory/MICRON_4Gb_DDR4-2400_8bit_A.xml
wide_io_path=config/memory/JEDEC_256Mb_WIDEIO_SDR-266_128bit.xml
zedboard_path=config/memory/zedboard.xml

resnet18_network=config/network/resnet18.xml
resnet18_featuremap=featuremaps/resnet18_8b.h5

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
run $ddr3_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr3_baseline baseline
run $ddr3_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr3_bi bi
run $ddr3_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr3_rle rle
run $ddr3_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr3_def def
run $ddr3_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr3_pbm pbm 
run $ddr3_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr3_huffman huffman 

run $ddr3_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr3_baseline baseline
run $ddr3_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr3_bi bi
run $ddr3_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr3_rle rle
run $ddr3_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr3_def def
run $ddr3_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr3_pbm pbm
run $ddr3_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr3_huffman huffman 

run $ddr3_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr3_baseline baseline
run $ddr3_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr3_bi bi
run $ddr3_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr3_rle rle
run $ddr3_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr3_def def
run $ddr3_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr3_pbm pbm 
run $ddr3_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr3_huffman huffman

run $ddr3_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr3_baseline baseline
run $ddr3_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr3_bi bi
run $ddr3_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr3_rle rle
run $ddr3_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr3_def def
run $ddr3_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr3_pbm pbm
run $ddr3_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr3_huffman huffman

# DDR4

run $ddr4_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr4_baseline baseline
run $ddr4_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr4_bi bi
run $ddr4_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr4_rle rle
run $ddr4_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr4_def def
run $ddr4_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr4_pbm pbm
run $ddr4_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_ddr4_huffman huffman 

run $ddr4_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr4_baseline baseline
run $ddr4_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr4_bi bi
run $ddr4_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr4_rle rle
run $ddr4_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr4_def def
run $ddr4_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr4_pbm pbm
run $ddr4_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_ddr4_huffman huffman 

run $ddr4_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr4_baseline baseline
run $ddr4_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr4_bi bi
run $ddr4_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr4_rle rle
run $ddr4_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr4_def def
run $ddr4_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr4_pbm pbm
run $ddr4_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_ddr4_huffman huffman

run $ddr4_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr4_baseline baseline
run $ddr4_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr4_bi bi
run $ddr4_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr4_rle rle
run $ddr4_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr4_def def
run $ddr4_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr4_pbm pbm
run $ddr4_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_ddr4_huffman huffman

# WIDE-IO

run $wide_io_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_wide_io_baseline baseline
run $wide_io_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_wide_io_bi bi
run $wide_io_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_wide_io_rle rle
run $wide_io_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_wide_io_def def
run $wide_io_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_wide_io_pbm pbm
run $wide_io_path $resnet18_featuremap $tpu $resnet18_network outputs/tpu_resnet18_wide_io_huffman huffman 
run $wide_io_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_wide_io_baseline baseline
run $wide_io_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_wide_io_bi bi
run $wide_io_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_wide_io_rle rle
run $wide_io_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_wide_io_def def
run $wide_io_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_wide_io_pbm pbm
run $wide_io_path $resnet18_featuremap $eyeriss $resnet18_network outputs/eyeriss_resnet18_wide_io_huffman huffman 
run $wide_io_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_wide_io_baseline baseline
run $wide_io_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_wide_io_bi bi
run $wide_io_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_wide_io_rle rle
run $wide_io_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_wide_io_def def
run $wide_io_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_wide_io_pbm pbm
run $wide_io_path $resnet18_featuremap $scnn $resnet18_network outputs/scnn_resnet18_wide_io_huffman huffman
run $wide_io_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_wide_io_baseline baseline
run $wide_io_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_wide_io_bi bi
run $wide_io_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_wide_io_rle rle
run $wide_io_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_wide_io_def def
run $wide_io_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_wide_io_pbm pbm
run $wide_io_path $resnet18_featuremap $shidiannao $resnet18_network outputs/shidiannao_resnet18_wide_io_huffman huffman


