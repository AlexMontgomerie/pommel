import csv
import os
import argparse
import sys
import xmltodict
import deepdish as dd
from absl import flags
from collections import OrderedDict 

sys.path.append('SCALE-Sim')

from scale import scale

def get_dimensions(featuremaps):
    # get dimensions of each layer
    dimensions = {}
    #get layers 
    layers = featuremaps["layers"]
    # iterate over layers
    for layer in layers:
        dimensions[layer] = featuremaps[layer].shape
    dimensions["data"] = featuremaps["data"].shape
    dimensions["output"] = featuremaps["output"].shape
    # return dimensions
    return dimensions

# generate scale sim config
def generate_scale_sim_config(accelerator_path, output_path):
    
    # load accelerator
    with open(accelerator_path,"r") as f:
        accelerator = xmltodict.parse(f.read())

    # create scale sim config
    scale_sim_config = """[general]
run_name = "{run_name}"

[architecture_presets]
ArrayHeight:    {array_height}
ArrayWidth:     {array_width}
IfmapSramSz:    {ifmap_sram_size}
FilterSramSz:   {filter_sram_size}
OfmapSramSz:    {ofmap_sram_size}
IfmapOffset:    0
FilterOffset:   10000000
OfmapOffset:    20000000
Dataflow:       {dataflow}
    """.format(
            run_name        =output_path,
            array_height    =accelerator["acceleratorspec"]["parameter"][1]["@value"],
            array_width     =accelerator["acceleratorspec"]["parameter"][2]["@value"],
            ifmap_sram_size =accelerator["acceleratorspec"]["parameter"][3]["@value"],
            filter_sram_size=accelerator["acceleratorspec"]["parameter"][4]["@value"],
            ofmap_sram_size =accelerator["acceleratorspec"]["parameter"][5]["@value"],
            dataflow        =accelerator["acceleratorspec"]["parameter"][0]["@value"]
    )

    # save to output
    with open(f"outputs/{output_path}/scale_sim_config.cfg", "w") as f:
        f.write(scale_sim_config)

# generate scale sim topology
def generate_scale_sim_topology(network_path, featuremaps_path, output_path):

    # open network path
    with open(network_path, "r") as f:
        network = xmltodict.parse(f.read())

    # open the featuremaps
    featuremaps = dd.io.load(featuremaps_path)
    
    # get dimensions
    dimensions = get_dimensions(featuremaps)

    # create scale sim topology
    topology_config = "Layer name, IFMAP Height, IFMAP Width, Filter Height, Filter Width, Channels, Num Filter, Strides,\n"
    #for partition in net
    for index, partition in enumerate(network["networkspec"]["partition"]):
        
        # get input dimensions
        input_dim  = dimensions[partition["parameter"][2]["@value"]]
        output_dim = dimensions[partition["parameter"][3]["@value"]]

        if len(input_dim) < 4:
            height = 1
            width  = 1
        else:
            height = input_dim[3]
            width  = input_dim[2]

        channels = input_dim[1]
        filters  = output_dim[1]

        kernel_size = partition["parameter"][0]["@value"]
        stride      = partition["parameter"][1]["@value"]

        topology_config += f"{index}, "
        topology_config += f"{height}, "
        topology_config += f"{width}, "
        topology_config += f"{kernel_size}, "
        topology_config += f"{kernel_size}, "
        topology_config += f"{channels}, "
        topology_config += f"{filters}, "
        topology_config += f"{stride},\n"

    # save to output
    with open(f"outputs/{output_path}/scale_sim_topology.csv", "w") as f:
        f.write(topology_config)

def update_network(accelerator_path, network_path, output_path):

    # load accelerator
    with open(accelerator_path,"r") as f:
        accelerator = xmltodict.parse(f.read())

    # clock frequency
    clk_freq = int(accelerator["acceleratorspec"]["parameter"][6]["@value"])

    # open network path
    with open(network_path, "r") as f:
        network = xmltodict.parse(f.read())

    # open scale sim results
    with open(f"outputs/{output_path}/scale_sim_topology_avg_bw.csv", "r") as f:
        bandwidth = csv.reader(f, delimiter=',')
        next(bandwidth, None)
        # iterate over partitions
        for index, row in enumerate(bandwidth):
            
            # bandwidth in
            bandwidth_in = OrderedDict()
            bandwidth_in["@id"]     = "bandwidth_in"
            bandwidth_in["@type"]   = "string"
            bandwidth_in["@value"]   = str(float(row[4])*clk_freq/1000)
        
            # bandwidth out        
            bandwidth_out = OrderedDict()
            bandwidth_out["@id"]     = "bandwidth_out"
            bandwidth_out["@type"]   = "string"
            bandwidth_out["@value"]   = str(float(row[6])*clk_freq/1000)

            # add to network
            network["networkspec"]["partition"][index]["parameter"].append(bandwidth_in)
            network["networkspec"]["partition"][index]["parameter"].append(bandwidth_out)

    # save back to network
    with open(f"outputs/{output_path}/network.xml", "w") as f:
        f.write(xmltodict.unparse(network, pretty=True))

if __name__ == "__main__":

    # parser
    parser = argparse.ArgumentParser(description="Generate Network Config")
    parser.add_argument("-n","--network",required=True, help="network config template path")
    parser.add_argument("-a","--accelerator",required=True, help="accelerator config path")
    parser.add_argument("-f","--featuremap",required=True, help="featuremap path")
    parser.add_argument("-o","--output",required=True, help="output path")
    args = parser.parse_args()

    # create output dir
    os.system(f"mkdir -p outputs/{args.output}")

    # create configs
    generate_scale_sim_config(args.accelerator, args.output)
    generate_scale_sim_topology(args.network, args.featuremap, args.output)

    # run scale sim
    os.system(f"python SCALE-Sim/scale.py -arch_config=outputs/{args.output}/scale_sim_config.cfg -network=outputs/{args.output}/scale_sim_topology.csv")

    # update network
    update_network(args.accelerator, args.network, args.output)

    os.system("rm scale_sim_*")
