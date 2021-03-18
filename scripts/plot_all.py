import matplotlib.pyplot as plt
import json
import numpy as np
from scipy.interpolate import griddata
import metrics

# all networks
networks = [
    "alexnet",
    "vgg11",
    #"mobilenet_v2",
]

# all accelerators
accelerators = [
    "tpu",
    #"scnn",
    #"shidiannao",
    #"eyeriss",
]

# all memory types
memories = [
    "ddr3",
    #"ddr4",
    #"wide_io",
]

# all coding schemes
coding_schemes = [
    "baseline",
    "bi",
    "huffman",
    "rle",
    "def",
    "pbm",
]

# plot power against total execution time for all accelerators for mobilenet_v2 on DDR3
"""
for memory in memories:
    x = []
    y = []
    for accelerator in accelerators:
        # open report
        with open(f"outputs/{accelerator}_alexnet_{memory}_baseline/report.json", "r") as f:
            report = json.load(f)
        # get average power
        average_power = max(metrics.get_average_power(report,"in"),metrics.get_average_power(report,"out"))
        # get total run time
        run_time = max(np.sum(metrics.get_sequence(report,"samples","in")),np.sum(metrics.get_sequence(report,"samples","out")))
        # get x and y values
        x.append(run_time)
        y.append(average_power)
    plt.scatter(x,y)
    plt.xlabel("Total Samples")
    plt.ylabel("Average Power (mW)")
    plt.show()
"""
# plot comparison of coding schemes for mobilenet_v2 on DDR3
for network in networks:
    for memory in memories:
        y = {}
        x = np.arange(len(accelerators))
        width = 0.9/len(coding_schemes)
        for coding_scheme in coding_schemes:
            y[coding_scheme] = []
            for accelerator in accelerators:
                # open report
                with open(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json", "r") as f:
                    report = json.load(f)
                # get average power
                average_power = max(metrics.get_average_power(report,"in"),metrics.get_average_power(report,"out"))
                y[coding_scheme].append(average_power)
            # create bar plot
            plt.bar(x+coding_schemes.index(coding_scheme)/len(coding_schemes), y[coding_scheme], width, label=coding_scheme)
        plt.xticks(x,accelerators)
        plt.xlabel("Accelerator")
        plt.ylabel("Average Power (mW)")
        plt.title(f"Average Power for {network} on {memory}")
        plt.legend()
        plt.show()

# plot layer-wise power for different coding schemes for mobilenet_v2, TPU, DDR3
for accelerator in accelerators:
    for memory in memories:
        for network in networks:
            for coding_scheme in coding_schemes:
                # open report
                with open(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json", "r") as f:
                    report = json.load(f)
                # get sequence of total power
                io_power = metrics.get_total_io_power_sequence(report, "in")
                dram_power = metrics.get_total_dram_power_sequence(report, "in")
                total_power = io_power + dram_power
                plt.plot(np.arange(len(total_power)),total_power,label=coding_scheme)
            plt.xlabel("Partition Index")
            plt.ylabel("Average Power (mW)")
            plt.title(f"Average Power per layer for {accelerator} running {network} on {memory}")
            plt.legend()
            plt.show() 

# plot layer-wise Bandwidth for different coding schemes for mobilenet_v2, TPU, DDR3
for accelerator in accelerators:
    for network in networks:
        for coding_scheme in coding_schemes:
            # open report
            with open(f"outputs/{accelerator}_{network}_ddr3_{coding_scheme}/report.json", "r") as f:
                report = json.load(f)
            bandwidth = metrics.get_sequence(report,"samples","in")
            plt.plot(np.arange(len(bandwidth)),bandwidth,label=coding_scheme)
        plt.xlabel("Partition Index")
        plt.ylabel("Bandwidth (GB/s)")
        plt.title(f"Bandwidth per layer for {accelerator} running {network}")
        plt.legend()
        plt.show() 

# plot layer-wise compression for different coding schemes for mobilenet_v2, TPU, DDR3
for network in networks:
    for coding_scheme in coding_schemes:
        # open report
        with open(f"outputs/tpu_{network}_ddr3_{coding_scheme}/report.json", "r") as f:
            report = json.load(f)
        # get sequence of total power
        compression_ratio = metrics.get_sequence(report, "compression_ratio", direction="in")
        plt.plot(np.arange(len(compression_ratio)),compression_ratio,label=coding_scheme)
    plt.xlabel("layers")
    plt.ylabel("Compression Ratio")
    plt.title(f"{network}")
    plt.legend()
    plt.show() 

# plot layer-wise activity for different coding schemes for mobilenet_v2, TPU, DDR3
for coding_scheme in coding_schemes:
    # open report
    with open(f"outputs/tpu_alexnet_ddr3_{coding_scheme}/report.json", "r") as f:
        report = json.load(f)
    # get sequence of total power
    activity = metrics.get_sequence(report, "data_activity", direction="out")
    plt.plot(np.arange(len(activity)),activity,label=coding_scheme)
plt.legend()
plt.show() 
