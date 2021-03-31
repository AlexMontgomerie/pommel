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
    "lpddr2",
    #"lpddr3",
    "ddr3",
    "ddr3l",
    "ddr4",
    "wide_io",
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
for network in networks:
    for accelerator in accelerators:
        for memory in memories:
            for coding_scheme in coding_schemes:
                # get report
                report_path = f"outputs/{accelerator}_{network}_{memory}_{coding_scheme}/report.json"
                report = pommel.report(report_path)
                # update featuremap path
                if accelerator == "tpu":
                    report.report["featuremap_path"] = f"featuremaps/{network}_8b.h5"
                else:
                    report.report["featuremap_path"] = f"featuremaps/{network}_16b.h5"
                # save report
                report.save_report(report_path)
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

# create plot instance
plot = pommel.plot(networks, accelerators, memories, coding_schemes)

# create table of power model
plot.coefficients_table("eyeriss", "vgg11")

plot.layerwise_trace_length_bar_plot("vgg11", "lpddr2")
# plot average power
#plot.average_power_performance_scatter_plot("tpu", "vgg11")

# generate all plots
plot.average_power_coding_scheme_bar_plot()

#for accelerator in accelerators:
plot.layerwise_average_power_coding_scheme_plot("tpu")
    
plot.layerwise_bandwidth_coding_scheme_plot("ddr3")

plot.layerwise_bandwidth_ratio_coding_scheme_plot("ddr3")

plot.layerwise_compression_ratio_coding_scheme_plot("tpu", "ddr3")

plot.layerwise_activity_coding_scheme_plot("tpu", "ddr3")



