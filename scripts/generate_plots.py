import pommel

# all networks
networks = [
    "alexnet",
    #"vgg11",
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
    "pbm",
]

# create plot instance
plot = pommel.plot(networks, accelerators, memories, coding_schemes)


plot.average_power_performance_scatter_plot("alexnet")

# generate all plots
plot.average_power_coding_scheme_bar_plot()

#for accelerator in accelerators:
#    plot.layerwise_average_power_coding_scheme_plot(accelerator)
    
plot.layerwise_bandwidth_coding_scheme_plot("ddr3")

plot.layerwise_bandwidth_ratio_coding_scheme_plot("ddr3")

plot.layerwise_compression_ratio_coding_scheme_plot("tpu", "ddr3")

plot.layerwise_activity_coding_scheme_plot("tpu", "ddr3")


