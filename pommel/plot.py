import matplotlib.pyplot as plt
import json
import numpy as np
import matplotlib.style
import matplotlib as mpl
from cycler import cycler

import pommel

# setup plotting parameters
mpl.rcParams['axes.prop_cycle'] = cycler(color='bgrcmyk')

mpl.rcParams['grid.color'] = 'k'
mpl.rcParams['grid.linestyle'] = ':'
mpl.rcParams['grid.linewidth'] = 0.5

mpl.rcParams['figure.figsize'] = [8.0, 6.0]
mpl.rcParams['figure.dpi'] = 80
mpl.rcParams['savefig.dpi'] = 100

#mpl.rcParams['font.size'] = 20 
#mpl.rcParams['legend.fontsize'] = 'small'
#mpl.rcParams['figure.titlesize'] = 'small'

mpl.rcParams['legend.fancybox'] = False
#mpl.rcParams['legend.fontsize'] = 'large'
mpl.rcParams['legend.framealpha'] = None
mpl.rcParams['legend.edgecolor'] = 'inherit'

mpl.rcParams['patch.force_edgecolor'] = True
mpl.rcParams['patch.facecolor'] = 'b'

mpl.rcParams['lines.linewidth'] = 1.0
mpl.rcParams['lines.dashed_pattern'] = [6, 6]
mpl.rcParams['lines.dashdot_pattern'] = [3, 5, 1, 5]
mpl.rcParams['lines.dotted_pattern'] = [1, 3]
mpl.rcParams['lines.scale_dashes'] = False

class plot:
    def __init__(self, networks, accelerators, memories, coding_schemes):
        
        # save lists
        self.networks       = networks
        self.accelerators   = accelerators
        self.memories       = memories
        self.coding_schemes = coding_schemes

    def create_plot(self, fig, axs, xlabel, ylabel, title):

        # set axis labels
        for ax in axs.flat:
            ax.grid()
            ax.set(xlabel=xlabel, ylabel=ylabel)
            ax.label_outer()

        # place legend in the corner
        handles, labels = ax.get_legend_handles_labels()
        fig.legend(handles, labels, loc='upper right')

        # plot
        fig.suptitle(title)
        plt.show()


    def average_power_performance_scatter_plot(self):
        
        fig, ax = plt.subplots(1, sharey="row")
        for memory in self.memories:
            for network in self.networks:
                y = [] 
                x = []
                for coding_scheme in self.coding_schemes:
                    for accelerator in self.accelerators:
                        # open report
                        report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                        # get average power
                        average_power = report.get_average_power()
                        average_bandwidth = report.get_average_bandwidth()
                        y.append(average_power)
                        x.append(average_bandwidth)
                plt.scatter(x,y)
        plt.show()


    def average_power_coding_scheme_bar_plot(self):
        
        fig, axs = plt.subplots(len(self.memories),len(self.networks), sharey="row")
        for memory in self.memories:
            for network in self.networks:
                y = {}
                x = np.arange(len(self.accelerators))
                width = 0.9/len(self.coding_schemes)
                ax = axs[self.memories.index(memory), self.networks.index(network)]
                for coding_scheme in self.coding_schemes:
                    y[coding_scheme] = []
                    for accelerator in self.accelerators:
                        # open report
                        report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                        # get average power
                        average_power = report.get_average_power()
                        y[coding_scheme].append(average_power)
                    # create bar plot
                    ax.bar(x+self.coding_schemes.index(coding_scheme)/len(self.coding_schemes), y[coding_scheme], width, label=coding_scheme)
                ax.set_xticks(x)
                ax.set_xticklabels(self.accelerators)
                ax.set_title(f"{network} on {memory}")

        # create plot
        self.create_plot(fig, axs, "Accelerator", "Average Power (mW)", "Comparision of Average Power for Coding Schemes")

    def layerwise_average_power_coding_scheme_plot(self, accelerator):
        
        fig, axs = plt.subplots(len(self.memories),len(self.networks), sharey="row")
        for memory in self.memories:
            for network in self.networks:
                ax = axs[self.memories.index(memory), self.networks.index(network)]
                for coding_scheme in self.coding_schemes:
                    # open report
                    report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                    # get sequence of total power
                    io_power = report.get_total_io_power_sequence()
                    dram_power = report.get_total_dram_power_sequence()
                    total_power = io_power + dram_power
                    ax.plot(np.arange(len(total_power)),total_power,label=coding_scheme)
                ax.set_title(f"{network} on {memory}")

        # create plot
        self.create_plot(fig, axs, "Partition Index", "Average Power (mW)", "Comparision of Average Power for Coding Schemes across Layers")

    def layerwise_bandwidth_coding_scheme_plot(self, memory):

        fig, axs = plt.subplots(len(self.accelerators), len(self.networks))
        for accelerator in self.accelerators:
            for network in self.networks:
                ax = axs[self.accelerators.index(accelerator),self.networks.index(network)]
                for coding_scheme in self.coding_schemes:
                    # open report
                    report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                    bandwidth = report.get_sequence("bandwidth")
                    ax.plot(np.arange(len(bandwidth)),bandwidth,label=coding_scheme)
                ax.set_title(f"Bandwidth per layer for {accelerator} running {network}")

        # create plot
        self.create_plot(fig, axs, "Partition Index", "Bandwidth (GB/s)", "Comparision of Bandwidth for Coding Schemes across Layers")

    def layerwise_bandwidth_ratio_coding_scheme_plot(self, memory):

        fig, axs = plt.subplots(len(self.accelerators), len(self.networks))
        for accelerator in self.accelerators:
            for network in self.networks:
                ax = axs[self.accelerators.index(accelerator),self.networks.index(network)]
                for coding_scheme in self.coding_schemes:
                    # open report
                    report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                    bandwidth = report.get_sequence("bandwidth")
                    bandwidth_mem = report.memory.bandwidth
                    ax.plot(np.arange(len(bandwidth)),bandwidth/bandwidth_mem,label=coding_scheme)
                ax.set_title(f"Bandwidth Ratio per layer for {accelerator} running {network}")

        # create plot
        self.create_plot(fig, axs, "Partition Index", "Bandwidth Ratio", "Comparision of Bandwidth Ratio for Coding Schemes across Layers")

    
    def layerwise_compression_ratio_coding_scheme_plot(self, accelerator, memory):

        fig, axs = plt.subplots(len(self.networks))
        for network in self.networks:
            ax = axs[self.networks.index(network)]
            for coding_scheme in self.coding_schemes:
                # open report
                report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                # get sequence of total power
                compression_ratio = report.get_sequence("compression_ratio")
                ax.plot(np.arange(len(compression_ratio)),compression_ratio,label=coding_scheme)
                ax.set_title(f"Compression Ratio per layer for {accelerator} running {network}")

        # create plot
        self.create_plot(fig, axs, "Partition Index", "Compression Ratio", "Comparision of Compression Ratio for Coding Schemes across Layers")

    def layerwise_activity_coding_scheme_plot(self, accelerator, memory):

        fig, axs = plt.subplots(len(self.networks))
        for network in self.networks:
            ax = axs[self.networks.index(network)]
            for coding_scheme in self.coding_schemes:
                # open report
                report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                # get sequence of total power
                activity = report.get_sequence("data_activity")
                ax.plot(np.arange(len(activity)),activity,label=coding_scheme)
                ax.set_title(f"Activity per layer for {accelerator} running {network}")

        # create plot
        self.create_plot(fig, axs, "Partition Index", "Activity", "Comparision of Compression Ratio for Coding Schemes across Layers")

