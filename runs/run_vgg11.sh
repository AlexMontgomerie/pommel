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

vgg11_fm_8=featuremaps/vgg11_8b.h5
vgg11_fm_16=featuremaps/vgg11_16b.h5

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
vgg11_tpu=config/network/vgg11_tpu.xml
vgg11_eyeriss=config/network/vgg11_eyeriss.xml
vgg11_scnn=config/network/vgg11_scnn.xml
vgg11_shidiannao=config/network/vgg11_shidiannao.xml

# LPDDR2
run $lpddr3_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr3_baseline baseline
run $lpddr3_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr3_bi bi
run $lpddr3_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr3_rle rle
run $lpddr3_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr3_def def
run $lpddr3_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr3_pbm pbm 
run $lpddr3_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr3_huffman huffman 
run $lpddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr3_baseline baseline
run $lpddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr3_bi bi
run $lpddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr3_rle rle
run $lpddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr3_def def
run $lpddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr3_pbm pbm
run $lpddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr3_huffman huffman 
run $lpddr3_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr3_baseline baseline
run $lpddr3_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr3_bi bi
run $lpddr3_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr3_rle rle
run $lpddr3_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr3_def def
run $lpddr3_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr3_pbm pbm 
run $lpddr3_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr3_huffman huffman
run $lpddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr3_baseline baseline
run $lpddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr3_bi bi
run $lpddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr3_rle rle
run $lpddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr3_def def
run $lpddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr3_pbm pbm
run $lpddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr3_huffman huffman

# LPDDR2
run $lpddr2_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr2_baseline baseline
run $lpddr2_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr2_bi bi
run $lpddr2_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr2_rle rle
run $lpddr2_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr2_def def
run $lpddr2_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr2_pbm pbm 
run $lpddr2_path $vgg11_fm_8 $tpu $vgg11_tpu                outputs/tpu_vgg11_lpddr2_huffman huffman 
run $lpddr2_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr2_baseline baseline
run $lpddr2_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr2_bi bi
run $lpddr2_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr2_rle rle
run $lpddr2_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr2_def def
run $lpddr2_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr2_pbm pbm
run $lpddr2_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss       outputs/eyeriss_vgg11_lpddr2_huffman huffman 
run $lpddr2_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr2_baseline baseline
run $lpddr2_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr2_bi bi
run $lpddr2_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr2_rle rle
run $lpddr2_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr2_def def
run $lpddr2_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr2_pbm pbm 
run $lpddr2_path $vgg11_fm_16 $scnn $vgg11_scnn             outputs/scnn_vgg11_lpddr2_huffman huffman
run $lpddr2_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr2_baseline baseline
run $lpddr2_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr2_bi bi
run $lpddr2_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr2_rle rle
run $lpddr2_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr2_def def
run $lpddr2_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr2_pbm pbm
run $lpddr2_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_lpddr2_huffman huffman

# DDR3L
run $ddr3l_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3l_baseline baseline
run $ddr3l_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3l_bi bi
run $ddr3l_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3l_rle rle
run $ddr3l_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3l_def def
run $ddr3l_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3l_pbm pbm 
run $ddr3l_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3l_huffman huffman 

run $ddr3l_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3l_baseline baseline
run $ddr3l_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3l_bi bi
run $ddr3l_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3l_rle rle
run $ddr3l_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3l_def def
run $ddr3l_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3l_pbm pbm
run $ddr3l_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3l_huffman huffman 

run $ddr3l_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3l_baseline baseline
run $ddr3l_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3l_bi bi
run $ddr3l_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3l_rle rle
run $ddr3l_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3l_def def
run $ddr3l_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3l_pbm pbm 
run $ddr3l_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3l_huffman huffman

run $ddr3l_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3l_baseline baseline
run $ddr3l_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3l_bi bi
run $ddr3l_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3l_rle rle
run $ddr3l_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3l_def def
run $ddr3l_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3l_pbm pbm
run $ddr3l_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3l_huffman huffman

# DDR3
run $ddr3_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3_baseline baseline
run $ddr3_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3_bi bi
run $ddr3_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3_rle rle
run $ddr3_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3_def def
run $ddr3_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3_pbm pbm 
run $ddr3_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr3_huffman huffman 

run $ddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3_baseline baseline
run $ddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3_bi bi
run $ddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3_rle rle
run $ddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3_def def
run $ddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3_pbm pbm
run $ddr3_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr3_huffman huffman 

run $ddr3_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3_baseline baseline
run $ddr3_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3_bi bi
run $ddr3_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3_rle rle
run $ddr3_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3_def def
run $ddr3_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3_pbm pbm 
run $ddr3_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr3_huffman huffman


run $ddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3_baseline baseline
run $ddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3_bi bi
run $ddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3_rle rle
run $ddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3_def def
run $ddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3_pbm pbm
run $ddr3_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr3_huffman huffman

# DDR4

run $ddr4_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr4_baseline baseline
run $ddr4_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr4_bi bi
run $ddr4_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr4_rle rle
run $ddr4_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr4_def def
run $ddr4_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr4_pbm pbm
run $ddr4_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_ddr4_huffman huffman 

run $ddr4_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr4_baseline baseline
run $ddr4_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr4_bi bi
run $ddr4_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr4_rle rle
run $ddr4_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr4_def def
run $ddr4_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr4_pbm pbm
run $ddr4_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_ddr4_huffman huffman 

run $ddr4_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr4_baseline baseline
run $ddr4_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr4_bi bi
run $ddr4_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr4_rle rle
run $ddr4_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr4_def def
run $ddr4_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr4_pbm pbm
run $ddr4_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_ddr4_huffman huffman

run $ddr4_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr4_baseline baseline
run $ddr4_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr4_bi bi
run $ddr4_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr4_rle rle
run $ddr4_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr4_def def
run $ddr4_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr4_pbm pbm
run $ddr4_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_ddr4_huffman huffman

# WIDE-IO

run $wide_io_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_wide_io_baseline baseline
run $wide_io_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_wide_io_bi bi
run $wide_io_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_wide_io_rle rle
run $wide_io_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_wide_io_def def
run $wide_io_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_wide_io_pbm pbm
run $wide_io_path $vgg11_fm_8 $tpu $vgg11_tpu outputs/tpu_vgg11_wide_io_huffman huffman

run $wide_io_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_wide_io_baseline baseline
run $wide_io_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_wide_io_bi bi
run $wide_io_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_wide_io_rle rle
run $wide_io_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_wide_io_def def
run $wide_io_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_wide_io_pbm pbm
run $wide_io_path $vgg11_fm_16 $eyeriss $vgg11_eyeriss outputs/eyeriss_vgg11_wide_io_huffman huffman 

run $wide_io_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_wide_io_baseline baseline
run $wide_io_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_wide_io_bi bi
run $wide_io_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_wide_io_rle rle
run $wide_io_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_wide_io_def def
run $wide_io_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_wide_io_pbm pbm
run $wide_io_path $vgg11_fm_16 $scnn $vgg11_scnn outputs/scnn_vgg11_wide_io_huffman huffman

run $wide_io_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_wide_io_baseline baseline
run $wide_io_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_wide_io_bi bi
run $wide_io_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_wide_io_rle rle
run $wide_io_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_wide_io_def def
run $wide_io_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_wide_io_pbm pbm
run $wide_io_path $vgg11_fm_16 $shidiannao $vgg11_shidiannao outputs/shidiannao_vgg11_wide_io_huffman huffman

