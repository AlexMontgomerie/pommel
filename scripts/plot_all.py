import matplotlib.pyplot as plt
import json
import numpy as np
from scipy.interpolate import griddata
import metrics

# all networks
networks = [
    "mobilenet_v2",
]

# all accelerators
accelerators = [
    "tpu",
#    "scnn",
#    "shidiannao",
    "eyeriss",
]

# all memory types
memories = [
    "ddr3",
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
"""
# plot power against total execution time for all accelerators for mobilenet_v2 on DDR3
x = []
y = []
for accelerator in accelerators:
    # open report
    with open(f"outputs/{accelerator}_mobilenet_v2_ddr4_baseline/report.json", "r") as f:
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

# plot comparison of coding schemes for mobilenet_v2 on DDR3
y = {}
x = np.arange(len(accelerators))
width = 0.9/len(coding_schemes)
for coding_scheme in coding_schemes:
    y[coding_scheme] = []
    for accelerator in accelerators:
        # open report
        with open(f"outputs/{accelerator}_mobilenet_v2_ddr4_{coding_scheme}/report.json", "r") as f:
            report = json.load(f)
        # get average power
        average_power = max(metrics.get_average_power(report,"in"),metrics.get_average_power(report,"out"))
        y[coding_scheme].append(average_power)
    # create bar plot
    print(y)
    plt.bar(x+coding_schemes.index(coding_scheme)/len(coding_schemes), y[coding_scheme], width, label=coding_scheme)
plt.xticks(x,accelerators)
plt.legend()
plt.show()
"""
# plot layer-wise power for different coding schemes for mobilenet_v2, TPU, DDR3
for coding_scheme in coding_schemes:
    # open report
    with open(f"outputs/tpu_alexnet_ddr3_{coding_scheme}/report.json", "r") as f:
        report = json.load(f)
    # get sequence of total power
    io_power = metrics.get_total_io_power_sequence(report, "in")
    dram_power = metrics.get_total_dram_power_sequence(report, "in")
    total_power = io_power + dram_power
    plt.plot(np.arange(len(total_power)),total_power,label=coding_scheme)
plt.legend()
plt.show() 

# plot layer-wise compression for different coding schemes for mobilenet_v2, TPU, DDR3
for coding_scheme in coding_schemes:
    # open report
    with open(f"outputs/tpu_mobilenet_v2_ddr4_{coding_scheme}/report.json", "r") as f:
        report = json.load(f)
    # get sequence of total power
    compression_ratio = metrics.get_sequence(report, "compression_ratio", direction="in")
    plt.plot(np.arange(len(compression_ratio)),compression_ratio,label=coding_scheme)
plt.legend()
plt.show() 

# plot layer-wise activity for different coding schemes for mobilenet_v2, TPU, DDR3
for coding_scheme in coding_schemes:
    # open report
    with open(f"outputs/tpu_mobilenet_v2_ddr4_{coding_scheme}/report.json", "r") as f:
        report = json.load(f)
    # get sequence of total power
    activity = metrics.get_sequence(report, "data_activity", direction="out")
    plt.plot(np.arange(len(activity)),activity,label=coding_scheme)
plt.legend()
plt.show() 
