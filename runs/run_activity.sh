#!/bin/bash

# build tool TODO: remove later on
make clean
make -j 8

zc702_mem=config/memory/zc702.xml
activity_fm=featuremaps/activity_32b.h5
zc702=config/accelerator/zc702.xml
activity_bandwidth_network=config/network/activity_bandwidth_test.xml

mkdir -p outputs/activity_bandwidth

sh ./run_pommel.sh $zc702_mem $activity_fm baseline $activity_bandwidth_network $zc702 outputs/activity_bandwidth


