import matplotlib.pyplot as plt
import json
import numpy as np
from scipy.interpolate import griddata
#import metrics

import pommel

# all networks
networks = [
    "alexnet",
    "vgg11",
    "resnet18",
    #"mobilenet_v2",
]

# all accelerators
accelerators = [
    "tpu",
    "scnn",
    "shidiannao",
    "eyeriss",
]

# all memory types
memories = [
    "ddr3",
    "ddr4",
    #"wide_io",
    #"zedboard",
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

plot = pommel.plot(networks, accelerators, memories, coding_schemes)
plot.average_power_coding_scheme_bar_plot()

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
fig, axs = plt.subplots(len(networks), len(memories))
for network in networks:
    for memory in memories:
        y = {}
        x = np.arange(len(accelerators))
        width = 0.9/len(coding_schemes)
        ax = axs[networks.index(network),memories.index(memory)]
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
            ax.bar(x+coding_schemes.index(coding_scheme)/len(coding_schemes), y[coding_scheme], width, label=coding_scheme)
        ax.set_xticks(x)
        ax.set_xticklabels(accelerators)
        ax.set_title(f"{network} on {memory}")

# set axis labels
for ax in axs.flat:
    ax.set(xlabel="Accelerator", ylabel="Average Power (mW)")
    ax.label_outer()

# plot
plt.legend()
plt.show()

# plot layer-wise power for different coding schemes for mobilenet_v2, TPU, DDR3
for accelerator in accelerators:
    fig, axs = plt.subplots(len(networks), len(memories))
    for memory in memories:
        for network in networks:
            ax = axs[networks.index(network),memories.index(memory)]
            for coding_scheme in coding_schemes:
                # open report
                with open(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json", "r") as f:
                    report = json.load(f)
                # get sequence of total power
                io_power = metrics.get_total_io_power_sequence(report, "in")
                dram_power = metrics.get_total_dram_power_sequence(report, "in")
                total_power = io_power + dram_power
                ax.plot(np.arange(len(total_power)),total_power,label=coding_scheme)
            ax.set_title(f"{accelerator} running {network} on {memory}")

    # set axis labels
    for ax in axs.flat:
        ax.set(xlabel="Partition Index", ylabel="Average Power (mW)")
        ax.label_outer()

    # plot 
    plt.legend()
    plt.show()


# plot layer-wise Bandwidth for different coding schemes for mobilenet_v2, TPU, DDR3
fig, axs = plt.subplots(len(accelerators), len(memories))
for accelerator in accelerators:
    for network in networks:
        ax = axs[accelerators.index(accelerator),networks.index(network)]
        for coding_scheme in coding_schemes:
            # open report
            with open(f"outputs/{accelerator}_{network}_ddr3_{coding_scheme}/report.json", "r") as f:
                report = json.load(f)
            bandwidth = metrics.get_sequence(report,"bandwidth","in")
            ax.plot(np.arange(len(bandwidth)),bandwidth,label=coding_scheme)
        ax.set_title(f"Bandwidth per layer for {accelerator} running {network}")

# set axis labels
for ax in axs.flat:
    ax.set(xlabel="Partition Index", ylabel="Bandwidth (GB/s)")
    ax.label_outer()

# plot 
plt.legend()
plt.show()

# plot layer-wise Bandwidth ratio for different coding schemes for mobilenet_v2, TPU, DDR3
for accelerator in accelerators:
    for network in networks:
        for coding_scheme in coding_schemes:
            # open report
            with open(f"outputs/{accelerator}_{network}_ddr3_{coding_scheme}/report.json", "r") as f:
                report = json.load(f)
            bandwidth = metrics.get_sequence(report,"bandwidth","in")
            bandwidth_mem = metrics.get_base_sequence(report,"mem_bandwidth")
            plt.plot(np.arange(len(bandwidth)),bandwidth/bandwidth_mem,label=coding_scheme)
        plt.xlabel("Partition Index")
        plt.ylabel("Bandwidth Ratio")
        plt.title(f"Bandwidth Ratio to Memory per layer for {accelerator} running {network}")
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
