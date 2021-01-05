from xml.dom import minidom
import deepdish as dd
import numpy as np
import untangle
import argparse

# function to get dimensions of all the featuremaps
def get_layers(filepath):
    # load file
    featuremaps = dd.io.load(filepath)
    # return keys
    return featuremaps["layers"]

def get_dimensions(featuremaps):
    # get dimensions of each layer
    dimensions = {}
    #get layers 
    layers = featuremaps["layers"]
    # iterate over layers
    for layer in layers:
        dimensions[layer] = featuremaps[layer].shape
    # return dimensions
    return dimensions


def add_bi_parameters(root, layer, partition_info, featuremaps):
    # add bitwidth
    parameter = root.createElement('parameter')
    parameter.setAttribute("id","bitwidth")
    parameter.setAttribute("type","int")
    parameter.setAttribute("value",partition_info["bitwidth"])
    layer.appendChild(parameter)

def add_def_parameters(root, layer, partition_info, featuremaps):
    # get featuremap dimensions
    dimensions = get_dimensions(featuremaps)
    # add bitwidth
    parameter = root.createElement('parameter')
    parameter.setAttribute("id","bitwidth")
    parameter.setAttribute("type","int")
    parameter.setAttribute("value",partition_info["bitwidth"])
    layer.appendChild(parameter)
    # add distance
    parameter = root.createElement('parameter')
    parameter.setAttribute("id","distance")
    parameter.setAttribute("type","int")
    parameter.setAttribute("value",str(dimensions[layer.getAttribute("id")][1]))
    layer.appendChild(parameter)

def generate_encoder_config(encoder, accelerator_config_path, featuremaps_path, output_path):
    # create encoder config
    root = minidom.Document()
    encoderspec = root.createElement("encoderspec")
    encoderspec.setAttribute("type", encoder)
    root.appendChild(encoderspec)
    # load accelerator config
    accelerator_config = untangle.parse(accelerator_config_path)
    # load featuremaps
    featuremaps = dd.io.load(featuremaps_path)
    # iterate over partitions of the accelerator
    featuremaps_done = []
    for partition in accelerator_config.acceleratorspec.partition:
        # iterate over parameters in partition
        partition_info = {}
        for parameter in partition.parameter:
            partition_info[parameter["id"]] = parameter["value"]
        # add partition input featuremap to encoder spec
        if partition_info["input_featuremap"] not in featuremaps_done:
            featuremaps_done.append(partition_info["input_featuremap"])
            layer = root.createElement("layer")
            layer.setAttribute("id", partition_info["input_featuremap"])
            # add info for encoding scheme
            if encoder == "bi":
                add_bi_parameters(root, layer, partition_info, featuremaps)
            if encoder == "def":
                add_def_parameters(root, layer, partition_info, featuremaps)
            # add layer to encoderspec
            encoderspec.appendChild(layer)
        # add partition output featuremap to encoder spec
        if partition_info["output_featuremap"] not in featuremaps_done:
            featuremaps_done.append(partition_info["output_featuremap"])
            layer = root.createElement("layer")
            layer.setAttribute("id", partition_info["output_featuremap"])
            # add info for encoding scheme
            if encoder == "bi":
                add_bi_parameters(root, layer, partition_info, featuremaps)
            if encoder == "def":
                add_def_parameters(root, layer, partition_info, featuremaps)
            # add layer to encoderspec
            encoderspec.appendChild(layer)
    # save encoder config
    with open(output_path,"w") as f:
        f.write(root.toprettyxml(indent="\t"))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Encoder Config Generator Script")
    parser.add_argument("-e","--encoder",required=True, help="encoder name (def,bi,awr,abe,huffman,rle,pbm)")
    parser.add_argument("-a","--accelerator_config_path",required=True, help="accelerator config path (.xml)")
    parser.add_argument("-f","--featuremaps_path",required=True, help="featuremaps path (.h5)")
    parser.add_argument("-o","--output_path",required=True, help="output path (.xml)")

    args = parser.parse_args()
    generate_encoder_config(args.encoder,args.accelerator_config_path,args.featuremaps_path,args.output_path) 

    #generate_encoder_config("def","config/accelerator/fpgaconvnet.xml","data/test.h5","config/test_encoder.xml") 
