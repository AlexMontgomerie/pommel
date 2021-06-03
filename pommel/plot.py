import matplotlib.pyplot as plt
import json
import numpy as np
import matplotlib.style
import matplotlib as mpl
from cycler import cycler
from tabulate import tabulate

import pommel

# setup plotting parameters
mpl.rcParams['axes.prop_cycle'] = cycler(color='bgrcmyk')

mpl.rcParams['grid.color'] = 'k'
mpl.rcParams['grid.linestyle'] = ':'
mpl.rcParams['grid.linewidth'] = 0.5

mpl.rcParams['figure.figsize'] = [8.0, 6.0]
mpl.rcParams['figure.dpi'] = 80
mpl.rcParams['savefig.dpi'] = 100

mpl.rcParams['font.size'] = 15
mpl.rcParams['legend.fontsize'] = 'small'
mpl.rcParams['figure.titlesize'] = 'small'

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
            #ax.set(xlabel=xlabel, ylabel=ylabel)
            ax.label_outer()

        # place legend in the corner
        handles, labels = ax.get_legend_handles_labels()
        fig.legend(handles, labels, loc='upper right')

        # plot
        fig.suptitle(title)

        fig.add_subplot(111, frameon=False)
        plt.tick_params(labelcolor='none', which='both', top=False, bottom=False, left=False, right=False)

        plt.xlabel(xlabel,size="large")
        plt.ylabel(f"{ylabel}\n\n",size="large")

        plt.show()


    def accelerator_bar_plot(self, network):

        # create figure
        fig, axs = plt.subplots(2, sharey="row")

        x = np.arange(len(self.memories))
        width = 0.65/len(self.accelerators)

        # add performance bar plots
        performance = {}
        for accelerator in self.accelerators:
            performance[accelerator] = []
            for memory in self.memories:
                # load report
                report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_baseline/report.json")
                # get performance
                performance[accelerator].append(report.get_performance())
            # add bar plot
            #axs[0].bar(x + 0.5 + (self.accelerators.index(accelerator)-1)/len(self.accelerators), performance[accelerator], width, label=accelerator, align='center')
            #axs[0].set_ylabel("Performance (GOP/s)", size="medium")
            #axs[0].axes.xaxis.set_visible(False)
            #axs[0].grid(axis='y')

        # add bandwidth bar plots
        power = {}
        for accelerator in self.accelerators:
            power[accelerator] = []
            for memory in self.memories:
                # load report
                report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_baseline/report.json")
                # get performance
                power[accelerator].append(report.get_average_power())
            # add bar plot
            axs[0].bar(x + 0.5 + (self.accelerators.index(accelerator)-1)/len(self.accelerators)*0.8, power[accelerator], width, label=report.get_accelerator_name(), align='center')
            axs[0].set_ylabel("Avg. Power (mW)", size="medium")
            axs[0].axes.xaxis.set_visible(False)
            axs[0].grid(axis='y')

        # add bandwidth bar plots
        energy = {}
        for accelerator in self.accelerators:
            energy[accelerator] = []
            memory_names = []
            for memory in self.memories:
                # load report
                report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_baseline/report.json")
                # get performance
                energy[accelerator].append(report.get_total_energy())
                memory_names.append(report.get_memory_name())
            # add bar plot
            axs[1].bar(x + 0.5 + (self.accelerators.index(accelerator)-1)/len(self.accelerators)*0.8, energy[accelerator], width, label=report.get_accelerator_name(), align='center')
            axs[1].set_ylabel("Total Energy ($\mu$ J)", size="medium")
            axs[1].set_xticks(x+0.5)
            axs[1].set_xticklabels(memory_names)
            axs[1].grid(axis='y')

        handles, labels = axs[0].get_legend_handles_labels()
        fig.legend(handles, labels, loc="upper right")
        plt.show()


    def accelerator_power_energy_layerwise_plot(self, network, memory):

        # create figure
        fig, axs = plt.subplots(len(self.accelerators), sharey="col")
        axs2 = []

        for accelerator in self.accelerators:
            # get axis
            ax = axs[self.accelerators.index(accelerator)]
            # open report
            report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_baseline/report.json")
            # get power and energy sequence
            energy = report.get_energy_sequence()
            power = report.get_total_power_sequence()
            # add plots
            ax.plot(np.arange(len(energy)), energy, color="b", label="Energy")
            #ax2 = ax.twinx()
            #axs2.append(ax2)
            #ax2.plot(np.arange(len(energy)), power , color="r", label="Power")
            ax.set_ylabel(report.get_accelerator_name(),size="medium")
            ax.grid()
            ax.label_outer()

        ax.set_xlabel("Partition Index")
        #ax.grid()
        #ax.set(xlabel="Performance (GOP/s)", ylabel="Average Power (mW)")

        # place legend in the corner
        handles, labels = ax.get_legend_handles_labels()
        #handles2, labels2 = ax2.get_legend_handles_labels()
        #fig.legend(handles+handles2, labels+labels2, loc='lower center')
        fig.legend(handles, labels, loc='lower center')

        #for ax in axs2:
        #    ax.get_shared_y_axes().join(*axs2)


        ax = fig.add_subplot(111, frameon=False)

        ax.set_ylabel(f"Energy ($\mu$ J)\n\n",size="large")
        #ax2 = ax.twinx()
        #ax2.set_ylabel(f"Power (mW)",size="large", color="r")

        ax.tick_params(labelcolor='none', which='both', top=False, bottom=False, left=False, right=False)
        #ax2.tick_params(labelcolor='none', which='both', top=False, bottom=False, left=False, right=False)

        ax.spines["right"].set_visible(False)
        ax.spines["left"].set_visible(False)

        #ax2.spines["right"].set_visible(False)
        #ax2.spines["left"].set_visible(False)

        # plot
        #fig.suptitle(title)
        plt.show()

    def power_breakdown_layerwise_plot(self, network, accelerator):
        # create figure
        fig, axs = plt.subplots(len(self.memories), sharey="col")

        for memory in self.memories:
            # get axis
            ax = axs[self.memories.index(memory)]
            # open report
            report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_baseline/report.json")
            # get IO and DRAM power
            dram_power  = report.get_total_dram_power_sequence()
            io_power    = report.get_total_io_power_sequence()
            # add plots
            ax.plot(np.arange(len(dram_power)), dram_power, color="b", label="DRAM")
            ax.plot(np.arange(len(io_power)), io_power , color="b", linestyle="dashed", label="IO")
            ax.grid()
            ax.label_outer()
            ax.set_ylabel(report.get_memory_name())
            ax.set_xlim(left=0,right=len(io_power)-1)

        ax.set_xlabel("Partition Index")
        #ax.grid()
        #ax.set(xlabel="Performance (GOP/s)", ylabel="Average Power (mW)")

        # place legend in the corner
        handles, labels = ax.get_legend_handles_labels()
        fig.legend(handles, labels, loc='upper right')

        ax = fig.add_subplot(111, frameon=False)
        ax.set_ylabel(f"Power (mW)\n\n",size="large")
        ax.tick_params(labelcolor='none', which='both', top=False, bottom=False, left=False, right=False)

        plt.show()

    def average_power_performance_scatter_plot(self, network, accelerator):

        markers_all = ['o', 'v', '^', '<', '>', '8', 's', 'p', '*', 'h', 'H', 'D', 'd', 'P', 'X']
        #fig, axs = plt.subplots(len(self.accelerators), sharey="row")
        fig = plt.figure()
        ax = fig.add_subplot(111)
        ax.set_xscale("log")
        for memory in self.memories:
            y = []
            x = []
            markers = []
            # open report
            report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_baseline/report.json")
            #print(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
            #print("workload = ", report.get_total_workload())
            #print("performance = ",report.get_performance())
            # get average power
            total_energy = sum(report.get_energy_sequence())
            performance = report.get_performance()
            y.append(total_energy)
            x.append(performance)
            ax.scatter(x,y, label=report.get_memory_name(), marker="x")

        ax.grid()
        ax.set(xlabel="Performance (GOP/s)", ylabel="Average Power (mW)")

        # place legend in the corner
        handles, labels = ax.get_legend_handles_labels()
        fig.legend(handles, labels, loc='upper right')

        # plot
        #fig.suptitle(title)
        plt.show()


        #self.create_plot(fig, ax, "Accelerator", "Average Power (mW)", "Comparision of Average Power for Coding Schemes")

    def coding_scheme_bar_plot(self, accelerator):

        fig, axs = plt.subplots(2, len(self.networks), sharey="row")
        for network in self.networks:
            power = {}
            network_names = []
            x = np.arange(len(self.memories))
            width = 0.65/len(self.coding_schemes)
            ax = axs[0, self.networks.index(network)]
            for coding_scheme in self.coding_schemes:
                power[coding_scheme] = []
                memory_names = []
                for memory in self.memories:
                    # open report
                    report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                    # get average power
                    average_power = report.get_average_power()
                    power[coding_scheme].append(average_power)
                    memory_names.append(report.get_memory_name())
                # create bar plot
                ax.bar(x+0.5+(self.coding_schemes.index(coding_scheme)-1)/len(self.coding_schemes)*0.75, power[coding_scheme], width, label=coding_scheme.upper())
            ax.set_xticks(x+0.5)
            ax.set_xticklabels(memory_names)

            energy = {}
            x = np.arange(len(self.memories))
            width = 0.65/len(self.coding_schemes)
            ax = axs[1, self.networks.index(network)]
            for coding_scheme in self.coding_schemes:
                energy[coding_scheme] = []
                memory_names = []
                for memory in self.memories:
                    # open report
                    report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                    # get average power
                    total_energy = report.get_total_energy()
                    energy[coding_scheme].append(total_energy)
                    memory_names.append(report.get_memory_name())
                # create bar plot
                ax.bar(x+0.5+(self.coding_schemes.index(coding_scheme)-1)/len(self.coding_schemes)*0.75, energy[coding_scheme], width, label=coding_scheme.upper())
            ax.set_xticks(x+0.5)
            ax.set_xticklabels(memory_names)

            #ax.set_title(f"{network} on {memory}")

        # set titles
        for ax, network in zip(axs[0], self.networks):
            # open report
            report = pommel.report(f"outputs/tpu_{network}_ddr3_baseline/report.json")
            ax.set_title(report.get_network_name())

        axs[0,0].set_ylabel("Power (mW)", size="medium")
        axs[1,0].set_ylabel("Energy ($\mu$ J)", size="medium")

        # create plot
        #self.create_plot(fig, axs, "Accelerator", "Average Power (mW)", "Comparision of Average Power for Coding Schemes")
        self.create_plot(fig, axs, "", "", "")

    def coding_scheme_plot(self, accelerator, memory, network):

        fig, axs = plt.subplots(2)
        power = {}
        x = np.arange(len(self.memories))
        width = 0.9/len(self.coding_schemes)
        ax = axs[0]
        ax.grid()
        ax.label_outer()
        for coding_scheme in self.coding_schemes:
            # open report
            report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
            # get average power
            power[coding_scheme] = report.get_total_power_sequence()
            # create bar plot
            ax.plot(np.arange(len(power[coding_scheme])), power[coding_scheme], label=coding_scheme.upper())
        ax.set_xlim(left=0,right=len(power[coding_scheme])-1)

        report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
        ax2 = ax.twinx()
        bandwidth = report.get_base_sequence("bandwidth")
        ax2.bar(np.arange(len(bandwidth)), bandwidth, alpha=0.2)
        ax2.set_ylabel("Avg. Bandwidth (GB/s)", size="medium")

        energy = {}
        x = np.arange(len(self.memories))
        width = 0.9/len(self.coding_schemes)
        ax = axs[1]
        ax.grid()
        ax.label_outer()
        for coding_scheme in self.coding_schemes:
            # open report
            report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
            # get average power
            energy[coding_scheme] = report.get_energy_sequence()
            # create bar plot
            ax.plot(np.arange(len(energy[coding_scheme])), energy[coding_scheme], label=coding_scheme.upper())
        ax.set_xlim(left=0,right=len(power[coding_scheme])-1)

        report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
        ax2 = ax.twinx()
        latency = report.get_latency_sequence()*1000
        ax2.bar(np.arange(len(latency)), latency, alpha=0.2)
        ax2.set_ylabel("Latency (ms)", size="medium")


        axs[0].set_ylabel("Power (mW)", size="medium")
        axs[1].set_ylabel("Energy ($\mu$ J)", size="medium")

        handles, labels = axs[1].get_legend_handles_labels()
        fig.legend(handles, labels, bbox_to_anchor=(10,800), bbox_transform=axs[0].transData)
        axs[1].set_xlabel("Partition Index",size="large")
        plt.show()

        # create plot
        #self.create_plot(fig, axs, "Accelerator", "Average Power (mW)", "Comparision of Average Power for Coding Schemes")
        #self.create_plot(fig, axs, "Partition Index", "", "")


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
                        #print(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                        # get average power
                        average_power = report.get_average_power()
                        y[coding_scheme].append(average_power)
                    # create bar plot
                    ax.bar(x+self.coding_schemes.index(coding_scheme)/len(self.coding_schemes), y[coding_scheme], width, label=coding_scheme)
                ax.set_xticks(x)
                ax.set_xticklabels(self.accelerators)
                #ax.set_title(f"{network} on {memory}")

        # set titles
        for ax, network in zip(axs[0], self.networks):
            ax.set_title(network)

        for ax, memory in zip(axs[:,0], self.memories):
            ax.set_ylabel(memory,size="medium")

        # create plot
        #self.create_plot(fig, axs, "Accelerator", "Average Power (mW)", "Comparision of Average Power for Coding Schemes")
        self.create_plot(fig, axs, "Accelerator", "Average Power (mW)", "")

    def layerwise_average_power_coding_scheme_plot(self, accelerator):

        fig, axs = plt.subplots(len(self.memories),len(self.networks), sharey="row")
        for memory in self.memories:
            for network in self.networks:
                ax = axs[self.memories.index(memory), self.networks.index(network)]
                for coding_scheme in self.coding_schemes:
                    # open report
                    report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                    # get sequence of total power
                    total_power = report.get_total_power_sequence()
                    ax.plot(np.arange(len(total_power)),total_power,label=coding_scheme)
                #ax.set_title(f"{network} on {memory}")

        # set titles
        for ax, network in zip(axs[0], self.networks):
            ax.set_title(network)

        for ax, memory in zip(axs[:,0], self.memories):
            ax.set_ylabel(memory,size="medium")

    def layerwise_energy_coding_scheme_plot(self, accelerator, network):

        fig, axs = plt.subplots(len(self.memories))
        for memory in self.memories:
            ax = axs[self.memories.index(memory)]
            # open report
            report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_baseline/report.json")
            # get partitions
            partitions = report.get_partition_sequence()
            # get sequence of energy
            total_energy = report.get_energy_sequence()
            ax.plot(partitions, total_energy, color="r",label="Energy")
            ax2 = ax.twinx()
            # get power and
            dram_power = report.get_total_dram_power_sequence()
            io_power = report.get_total_io_power_sequence()
            ax2.plot(partitions, dram_power, color="b",linestyle="dotted", label="Power (DRAM)")
            ax2.plot(partitions, io_power, color="b",linestyle="dashed", label="Power (IO)")
            #ax2.yaxis.set_visible(False)

        # add memory labels
        for ax, memory in zip(axs, self.memories):
            ax.set_ylabel(memory.upper(),size="medium")

        # set axis labels
        for ax in axs:
            ax.grid()
            ax.label_outer()

        # place legend in the corner
        handles, labels = ax.get_legend_handles_labels()
        handles2, labels2 = ax2.get_legend_handles_labels()
        fig.legend(handles+handles2, labels+labels2, loc='lower center')

        # add y labels
        ax = fig.add_subplot(111, frameon=False)
        plt.tick_params(labelcolor='none', which='both', top=False, bottom=False, left=False, right=False)

        # add x label
        ax.set_xlabel("Partition Index",size="large")
        ax.set_ylabel("Energy ($\mu$ J)\n\n",size="large")
        ax2 = ax.twinx()
        ax2.set_ylabel("Power (mW)\n\n\n\n\n\n",size="large",rotation=270, color="b")

        plt.show()


        # create plot
        #self.create_plot(fig, axs, "Partition Index", "Average Power (mW)", "Comparision of Average Power for Coding Schemes across Layers")
        self.create_plot(fig, axs, "Partition Index", "Average Power (mW)", "")

    def layerwise_average_power_plot(self, memory):

        fig, axs = plt.subplots(len(self.accelerators),len(self.networks), sharey="row")
        for accelerator in self.accelerators:
            for network in self.networks:
                ax = axs[self.accelerators.index(accelerator), self.networks.index(network)]
                # open report
                report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_baseline/report.json")
                # get sequence of total power
                total_power = report.get_total_power_sequence()
                ax.plot(np.arange(len(total_power)),total_power, color="r")
                ax2 = ax.twinx()
                # trace length percentage
                trace_length = report.get_dram_sequence("trace_length")
                total_length = np.sum(trace_length)
                trace_length = trace_length / total_length
                ax2.bar(np.arange(len(trace_length)),trace_length, alpha=0.25, color="b")
                ax2.yaxis.set_visible(False)

        # set titles
        for ax, network in zip(axs[0], self.networks):
            ax.set_title(network)

        for ax, accelerator in zip(axs[:,0], self.accelerators):
            ax.set_ylabel(accelerator,size="medium")

        # create plot
        #self.create_plot(fig, axs, "Partition Index", "Average Power (mW)", "Comparision of Average Power across Layers")
        self.create_plot(fig, axs, "Partition Index", "Average Power (mW)", "")

    def layerwise_trace_length_bar_plot(self, network, memory):

        fig, axs = plt.subplots(len(self.coding_schemes), len(self.accelerators), sharey="row")
        for accelerator in self.accelerators:
            for coding_scheme in self.coding_schemes:
                ax = axs[self.coding_schemes.index(coding_scheme), self.accelerators.index(accelerator)]
                # open report
                report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                # get sequence of total power
                trace_length = report.get_dram_sequence("trace_length")
                total_length = np.sum(trace_length)
                trace_length = trace_length / total_length
                ax.bar(np.arange(len(trace_length)),trace_length)
                ax.set_title(f"{coding_scheme}")

        # create plot
        self.create_plot(fig, axs, "Partition Index", "Trace Length (cycles)", "Comparision of Trace Length for Coding Schemes across Layers")

    def layerwise_bandwidth_coding_scheme_plot(self, memory):

        fig, axs = plt.subplots(len(self.accelerators), len(self.networks))
        for accelerator in self.accelerators:
            for network in self.networks:
                ax = axs[self.accelerators.index(accelerator),self.networks.index(network)]
                for coding_scheme in self.coding_schemes:
                    # open report
                    report = pommel.report(f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json")
                    bandwidth = report.get_base_sequence("bandwidth")
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
                compression_ratio_in  = report.get_sequence("compression_ratio","in")
                compression_ratio_out = report.get_sequence("compression_ratio","out")
                compression_ratio = 0.5*(compression_ratio_in + compression_ratio_out)
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
                activity = report.get_average_activity_sequence()
                ax.plot(np.arange(len(activity)),activity,label=coding_scheme)
                ax.set_title(f"Activity per layer for {accelerator} running {network}")

        # create plot
        self.create_plot(fig, axs, "Partition Index", "Activity", "Comparision of Compression Ratio for Coding Schemes across Layers")

    def coefficients_table(self, accelerator, network):

        # get table headers
        headers = ["Memory Type", "Static Power (mW)", "Bandwidth Coefficient (mW/GB/s)", "Activity Coefficient (mw/GT/s)"]
        table = []

        # iterate over memory types
        for memory in self.memories:
            # open report
            report = pommel.report(f"outputs/activity_bandwidth_{memory}/report.json")
            # get coefficents
            static_term, bandwidth_coefficient, activity_coefficient = report.get_power_model()
            table.append([memory, static_term, bandwidth_coefficient, activity_coefficient])

        # output table
        print(tabulate(table, headers=headers))
