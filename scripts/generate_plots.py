import numpy as np
import math
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
#    "tpu",
    "eyeriss",
    "scnn",
    "shidiannao",
]

# all memory types
memories = [
    "ddr3",
    "ddr3l",
    "ddr4",
    "lpddr2",
    "lpddr3",
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
    #"pbm",
]

"""
for memory in memories:
    for accelerator in accelerators:
        # get report
        report_path = f"outputs/{accelerator}_resnet18_{memory}_baseline/report.json"
        report = pommel.report(report_path)
        print(
                memory,
                accelerator,
                "%.2f %.2f %.2f %.1f %.2f" % (
                report.get_performance(),
                report.get_average_bandwidth()*1000,
                report.get_average_activity(),
                report.get_average_power(),
                report.get_total_energy() )
        )
        print(  
                memory,
                report.memory.num_dq,
                report.memory.clock*report.memory.data_rate,
                report.memory.clock,
                report.memory.bandwidth,
        )


for network in networks:
    # open report
    report_path = f"outputs/tpu_{network}_ddr4_baseline/report.json"
    report = pommel.report(report_path)
    # get dims
    #print(network, report.get_featuremap_dims(), report.get_filter_dims())
    #fm_dims = 256*np.array(report.get_featuremap_dims())
    #filter_dims = np.array(report.get_filter_dims())
    #print(network, (fm_dims/filter_dims))
"""
energy_savings_all = []
power_savings_all = []
for network in networks:
    for accelerator in accelerators:
        for memory in memories:
            report_path = f"outputs/{accelerator}_{network}_{memory}_baseline/report.json"
            report = pommel.report(report_path)
            baseline_energy = report.get_total_energy() 
            baseline_power  = report.get_average_power()
            for coding_scheme in coding_schemes:
                # get report
                report_path = f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json"
                report = pommel.report(report_path)
                # update featuremap path
                """
                if accelerator == "tpu":
                    report.report["featuremap_path"] = f"featuremaps/{network}_8b.h5"
                else:
                    report.report["featuremap_path"] = f"featuremaps/{network}_16b.h5"
                # save report
                report.save_report(report_path)
                """
                energy = report.get_total_energy()
                power = report.get_average_power()
                energy_savings = (baseline_energy-energy)/baseline_energy
                power_savings = (baseline_power-power)/baseline_power

                energy_savings_all.append(energy_savings)
                power_savings_all.append(power_savings)
                #print(f"energy={energy_savings*100} power={power_savings*100}")

print(f"energy={max(energy_savings_all)} power={max(power_savings_all)}")

"""
for network in networks:
    for accelerator in accelerators:
        for memory in memories:
            for coding_scheme in coding_schemes:
                # get report
                report_path = f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json"
                report = pommel.report(report_path)
                # get max bandwidth
                max_bw = report.get_max_bandwidth()
                mem_bw = report.memory.bandwidth
                print(max_bw, mem_bw)
"""

# create plot instance
plot = pommel.plot(networks, accelerators, memories, coding_schemes)

plot.coefficients_table("eyeriss", "vgg11")
plot.accelerator_bar_plot("resnet18")
plot.coding_scheme_plot("tpu", "ddr4", "vgg11")
plot.coding_scheme_bar_plot("tpu")
plot.power_breakdown_layerwise_plot("mobilenet_v2", "tpu")
plot.accelerator_power_energy_layerwise_plot("vgg11", "ddr4")

"""
#plot.average_power_performance_scatter_plot("resnet18", "tpu")
# create table of power model
#plot.coefficients_table("eyeriss", "vgg11")

#plot.layerwise_trace_length_bar_plot("vgg11", "lpddr2")
# plot average power
#plot.average_power_performance_scatter_plot("tpu", "vgg11")

# generate all plots
#plot.average_power_coding_scheme_bar_plot()

plot.layerwise_energy_coding_scheme_plot("tpu","vgg11")

#for accelerator in accelerators:
#plot.layerwise_average_power_plot("ddr4")

plot.layerwise_average_power_coding_scheme_plot("tpu")
    
plot.layerwise_bandwidth_coding_scheme_plot("ddr3")

plot.layerwise_bandwidth_ratio_coding_scheme_plot("ddr3")

plot.layerwise_compression_ratio_coding_scheme_plot("tpu", "ddr3")

plot.layerwise_activity_coding_scheme_plot("tpu", "ddr3")
"""


