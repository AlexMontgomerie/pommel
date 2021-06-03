#!/bin/bash

# parameters
encoder=general
compression_ratios=(1.0 1.25 1.5 1.75 2.0 2.25 2.5 2.75 3.0)
activities=(0.1 0.5)
memories=(ddr3 ddr3l ddr4 lpddr2 lpddr3)
for memory in ${memories[@]}; do
    for compression_ratio in ${compression_ratios[@]}; do
        for activity in ${activities[@]}; do
            # set parameters
            memory_config=config/memory/${memory}.xml
            # featuremap_path=featuremaps/activity_32b.h5
            featuremap_path=featuremaps/alexnet_8b.h5
            accelerator_config=config/accelerator/tpu.xml
            # network_config=config/network/bandwidth_test.xml
            network_config=config/network/alexnet_tpu.xml

            # set output path
            output_path=outputs/bandwidth_memory_${memory}_cr_${compression_ratio}_activity_${activity}

            # run pommel
            sh ./run_pommel.sh $memory_config $featuremap_path $encoder $network_config $accelerator_config $output_path $compression_ratio $activity
        done
    done
done
