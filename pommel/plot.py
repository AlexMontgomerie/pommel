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
                        average_power = max(report.get_average_power("in"),report.get_average_power("out"))
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
                    io_power = report.get_total_io_power_sequence("in")
                    dram_power = report.get_total_dram_power_sequence("in")
                    total_power = io_power + dram_power
                    ax.plot(np.arange(len(total_power)),total_power,label=coding_scheme)
                ax.set_title(f"{network} on {memory}")

        # create plot
        self.create_plot(fig, axs, "Partition Index", "Average Power (mW)", "Comparision of Average Power for Coding Schemes across Layers")

