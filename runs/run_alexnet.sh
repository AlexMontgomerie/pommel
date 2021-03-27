#!/bin/bash

# build tool TODO: remove later on
make clean
make -j 8

ddr3_path=config/memory/ddr3.xml
ddr3l_path=config/memory/ddr3l.xml
ddr4_path=config/memory/ddr4.xml
lpddr2_path=config/memory/lpddr2.xml
lpddr3_path=config/memory/lpddr3.xml
wide_io_path=config/memory/wide_io.xml

alexnet_fm_8=featuremaps/alexnet_8b.h5
alexnet_fm_16=featuremaps/alexnet_16b.h5

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

# network paths
alexnet_tpu=config/network/alexnet_tpu.xml
alexnet_eyeriss=config/network/alexnet_eyeriss.xml
alexnet_scnn=config/network/alexnet_scnn.xml
alexnet_shidiannao=config/network/alexnet_shidiannao.xml

# LPDDR2
run $lpddr3_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr3_baseline baseline
run $lpddr3_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr3_bi bi
run $lpddr3_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr3_rle rle
run $lpddr3_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr3_def def
run $lpddr3_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr3_pbm pbm 
run $lpddr3_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr3_huffman huffman 
run $lpddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr3_baseline baseline
run $lpddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr3_bi bi
run $lpddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr3_rle rle
run $lpddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr3_def def
run $lpddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr3_pbm pbm
run $lpddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr3_huffman huffman 
run $lpddr3_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr3_baseline baseline
run $lpddr3_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr3_bi bi
run $lpddr3_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr3_rle rle
run $lpddr3_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr3_def def
run $lpddr3_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr3_pbm pbm 
run $lpddr3_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr3_huffman huffman
run $lpddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr3_baseline baseline
run $lpddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr3_bi bi
run $lpddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr3_rle rle
run $lpddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr3_def def
run $lpddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr3_pbm pbm
run $lpddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr3_huffman huffman

# LPDDR2
run $lpddr2_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr2_baseline baseline
run $lpddr2_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr2_bi bi
run $lpddr2_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr2_rle rle
run $lpddr2_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr2_def def
run $lpddr2_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr2_pbm pbm 
run $lpddr2_path $alexnet_fm_8 $tpu $alexnet_tpu                outputs/tpu_alexnet_lpddr2_huffman huffman 
run $lpddr2_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr2_baseline baseline
run $lpddr2_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr2_bi bi
run $lpddr2_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr2_rle rle
run $lpddr2_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr2_def def
run $lpddr2_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr2_pbm pbm
run $lpddr2_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss       outputs/eyeriss_alexnet_lpddr2_huffman huffman 
run $lpddr2_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr2_baseline baseline
run $lpddr2_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr2_bi bi
run $lpddr2_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr2_rle rle
run $lpddr2_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr2_def def
run $lpddr2_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr2_pbm pbm 
run $lpddr2_path $alexnet_fm_16 $scnn $alexnet_scnn             outputs/scnn_alexnet_lpddr2_huffman huffman
run $lpddr2_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr2_baseline baseline
run $lpddr2_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr2_bi bi
run $lpddr2_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr2_rle rle
run $lpddr2_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr2_def def
run $lpddr2_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr2_pbm pbm
run $lpddr2_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_lpddr2_huffman huffman

# DDR3L
run $ddr3l_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3l_baseline baseline
run $ddr3l_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3l_bi bi
run $ddr3l_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3l_rle rle
run $ddr3l_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3l_def def
run $ddr3l_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3l_pbm pbm 
run $ddr3l_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3l_huffman huffman 

run $ddr3l_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3l_baseline baseline
run $ddr3l_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3l_bi bi
run $ddr3l_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3l_rle rle
run $ddr3l_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3l_def def
run $ddr3l_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3l_pbm pbm
run $ddr3l_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3l_huffman huffman 

run $ddr3l_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3l_baseline baseline
run $ddr3l_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3l_bi bi
run $ddr3l_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3l_rle rle
run $ddr3l_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3l_def def
run $ddr3l_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3l_pbm pbm 
run $ddr3l_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3l_huffman huffman

run $ddr3l_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3l_baseline baseline
run $ddr3l_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3l_bi bi
run $ddr3l_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3l_rle rle
run $ddr3l_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3l_def def
run $ddr3l_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3l_pbm pbm
run $ddr3l_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3l_huffman huffman

# DDR3
run $ddr3_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3_baseline baseline
run $ddr3_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3_bi bi
run $ddr3_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3_rle rle
run $ddr3_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3_def def
run $ddr3_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3_pbm pbm 
run $ddr3_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr3_huffman huffman 

run $ddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3_baseline baseline
run $ddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3_bi bi
run $ddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3_rle rle
run $ddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3_def def
run $ddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3_pbm pbm
run $ddr3_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr3_huffman huffman 

run $ddr3_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3_baseline baseline
run $ddr3_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3_bi bi
run $ddr3_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3_rle rle
run $ddr3_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3_def def
run $ddr3_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3_pbm pbm 
run $ddr3_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr3_huffman huffman


run $ddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3_baseline baseline
run $ddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3_bi bi
run $ddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3_rle rle
run $ddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3_def def
run $ddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3_pbm pbm
run $ddr3_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr3_huffman huffman

# DDR4

run $ddr4_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr4_baseline baseline
run $ddr4_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr4_bi bi
run $ddr4_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr4_rle rle
run $ddr4_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr4_def def
run $ddr4_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr4_pbm pbm
run $ddr4_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_ddr4_huffman huffman 

run $ddr4_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr4_baseline baseline
run $ddr4_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr4_bi bi
run $ddr4_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr4_rle rle
run $ddr4_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr4_def def
run $ddr4_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr4_pbm pbm
run $ddr4_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_ddr4_huffman huffman 

run $ddr4_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr4_baseline baseline
run $ddr4_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr4_bi bi
run $ddr4_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr4_rle rle
run $ddr4_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr4_def def
run $ddr4_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr4_pbm pbm
run $ddr4_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_ddr4_huffman huffman

run $ddr4_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr4_baseline baseline
run $ddr4_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr4_bi bi
run $ddr4_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr4_rle rle
run $ddr4_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr4_def def
run $ddr4_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr4_pbm pbm
run $ddr4_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_ddr4_huffman huffman

# WIDE-IO

run $wide_io_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_wide_io_baseline baseline
run $wide_io_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_wide_io_bi bi
run $wide_io_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_wide_io_rle rle
run $wide_io_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_wide_io_def def
run $wide_io_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_wide_io_pbm pbm
run $wide_io_path $alexnet_fm_8 $tpu $alexnet_tpu outputs/tpu_alexnet_wide_io_huffman huffman

run $wide_io_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_wide_io_baseline baseline
run $wide_io_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_wide_io_bi bi
run $wide_io_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_wide_io_rle rle
run $wide_io_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_wide_io_def def
run $wide_io_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_wide_io_pbm pbm
run $wide_io_path $alexnet_fm_16 $eyeriss $alexnet_eyeriss outputs/eyeriss_alexnet_wide_io_huffman huffman 

run $wide_io_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_wide_io_baseline baseline
run $wide_io_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_wide_io_bi bi
run $wide_io_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_wide_io_rle rle
run $wide_io_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_wide_io_def def
run $wide_io_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_wide_io_pbm pbm
run $wide_io_path $alexnet_fm_16 $scnn $alexnet_scnn outputs/scnn_alexnet_wide_io_huffman huffman

run $wide_io_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_wide_io_baseline baseline
run $wide_io_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_wide_io_bi bi
run $wide_io_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_wide_io_rle rle
run $wide_io_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_wide_io_def def
run $wide_io_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_wide_io_pbm pbm
run $wide_io_path $alexnet_fm_16 $shidiannao $alexnet_shidiannao outputs/shidiannao_alexnet_wide_io_huffman huffman

