#!/bin/bash

# build tool TODO: remove later on
make clean
make -j 8

zc702_mem=config/memory/zc702.xml
activity_fm=featuremaps/activity_32b.h5
zc702=config/accelerator/zc702.xml
activity_bandwidth_network=config/network/activity_bandwidth_test.xml

ddr3_path=config/memory/ddr3.xml
ddr3l_path=config/memory/ddr3l.xml
ddr4_path=config/memory/ddr4.xml
lpddr2_path=config/memory/lpddr2.xml
lpddr3_path=config/memory/lpddr3.xml
wide_io_path=config/memory/wide_io.xml

mkdir -p outputs/activity_bandwidth

sh ./run_pommel.sh $ddr3_path       $activity_fm baseline $activity_bandwidth_network $zc702 outputs/activity_bandwidth_ddr3
sh ./run_pommel.sh $ddr3l_path      $activity_fm baseline $activity_bandwidth_network $zc702 outputs/activity_bandwidth_ddr3l
sh ./run_pommel.sh $ddr4_path       $activity_fm baseline $activity_bandwidth_network $zc702 outputs/activity_bandwidth_ddr4
sh ./run_pommel.sh $lpddr2_path     $activity_fm baseline $activity_bandwidth_network $zc702 outputs/activity_bandwidth_lpddr2
sh ./run_pommel.sh $lpddr3_path     $activity_fm baseline $activity_bandwidth_network $zc702 outputs/activity_bandwidth_lpddr3

#sh ./run_pommel.sh $zc702_mem $activity_fm baseline $activity_bandwidth_network $zc702 outputs/activity_bandwidth


