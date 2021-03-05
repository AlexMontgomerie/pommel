from xml.dom import minidom
import deepdish as dd
import numpy as np
import untangle
import argparse
import scipy.stats
import huffman

# function to get dimensions of all the featuremaps
def get_layers(filepath):
    # load file
    featuremaps = dd.io.load(filepath)
    # return keys
    layers = featuremaps["layers"] 
    layers.append["data"]
    return layers

def get_dimensions(featuremaps):
    # get dimensions of each layer
    dimensions = {}
    #get layers 
    layers = featuremaps["layers"]
    # iterate over layers
    for layer in layers:
        dimensions[layer] = featuremaps[layer].shape
    dimensions["data"] = featuremaps["data"].shape
    # return dimensions
    return dimensions

def get_frequency_table(featuremap):
    # frequency table initialised
    freq_table = {}
    # iterate over values in featuremap
    for x in featuremap.flatten():
        freq_table[str(x)] = freq_table.get(str(x),1) + 1
    # return frequency table
    return freq_table

def complete_frequency_table(freq_table, bitwidth=8):
    for i in range(2**bitwidth):
        freq_table[str(i)] = freq_table.get(str(i),1)
    return freq_table

def add_baseline_parameters(root, layer, platform_info, featuremaps):
    # add bitwidth
    parameter = root.createElement('parameter')
    parameter.setAttribute("id","null")
    parameter.setAttribute("type","int")
    parameter.setAttribute("value","0")
    layer.appendChild(parameter)

def add_bi_parameters(root, layer, platform_info, featuremaps):
    # add bitwidth
    parameter = root.createElement('parameter')
    parameter.setAttribute("id","bitwidth")
    parameter.setAttribute("type","int")
    parameter.setAttribute("value",platform_info["bitwidth"])
    layer.appendChild(parameter)

def add_pbm_parameters(root, layer, platform_info, featuremaps):
    # get the frequency table
    frequency_table = get_frequency_table(featuremaps[layer.getAttribute("id")])
    frequency_table = complete_frequency_table(frequency_table, int(platform_info["bitwidth"]))
    # add code table
    code_table_xml = root.createElement("code_table")
    for key, val in frequency_table.items():
        parameter = root.createElement("parameter")
        parameter.setAttribute("key", str(key))
        parameter.setAttribute("value", str(val))
        code_table_xml.appendChild(parameter)
    layer.appendChild(code_table_xml)

def add_rle_parameters(root, layer,  platform_info, featuremaps):
    # get the mode of the featuremap
    rle_zero = scipy.stats.mode(featuremaps[layer.getAttribute("id")].flatten())
    mask = ( 1 << int(platform_info["bitwidth"]) ) - 1
    rle_zero = rle_zero.mode[0] & mask
    # add rle zero
    parameter = root.createElement('parameter')
    parameter.setAttribute("id","rle_zero")
    parameter.setAttribute("type","int")
    parameter.setAttribute("value", str(rle_zero))
    layer.appendChild(parameter)

def add_huffman_parameters(root, layer,  platform_info, featuremaps):
    # get the frequency table
    frequency_table = get_frequency_table(featuremaps[layer.getAttribute("id")])
    frequency_table = complete_frequency_table(frequency_table, int(platform_info["bitwidth"]))
    # convert frequency table into code table
    huffman_inst = huffman.HuffmanCoding("")
    huffman_inst.make_heap(frequency_table)
    huffman_inst.merge_nodes()
    huffman_inst.make_codes()
    code_table = { key : int(val,2) for key,val in huffman_inst.codes.items() }
    # add code table
    code_table_xml = root.createElement("code_table")
    for key, val in code_table.items():
        parameter = root.createElement("parameter")
        parameter.setAttribute("key", str(key))
        parameter.setAttribute("value", str(val))
        code_table_xml.appendChild(parameter)
    layer.appendChild(code_table_xml)
    # add bitwidth
    parameter = root.createElement('parameter')
    parameter.setAttribute("id","bitwidth")
    parameter.setAttribute("type","int")
    parameter.setAttribute("value",platform_info["bitwidth"])
    layer.appendChild(parameter)

def add_def_parameters(root, layer, platform_info, featuremaps):
    # get featuremap dimensions
    dimensions = get_dimensions(featuremaps)
    # add bitwidth
    parameter = root.createElement('parameter')
    parameter.setAttribute("id","bitwidth")
    parameter.setAttribute("type","int")
    parameter.setAttribute("value",platform_info["bitwidth"])
    layer.appendChild(parameter)
    # add distance
    parameter = root.createElement('parameter')
    parameter.setAttribute("id","distance")
    parameter.setAttribute("type","int")
    parameter.setAttribute("value",str(dimensions[layer.getAttribute("id")][1]))
    layer.appendChild(parameter)

def generate_encoder_config(encoder, accelerator_config_path, network_config_path, featuremaps_path, output_path):
    # create encoder config
    root = minidom.Document()
    encoderspec = root.createElement("encoderspec")
    encoderspec.setAttribute("type", encoder)
    root.appendChild(encoderspec)
    # load accelerator config
    accelerator_config = untangle.parse(accelerator_config_path)
    platform_info = {}
    for parameter in accelerator_config.acceleratorspec.parameter:
        platform_info[parameter["id"]] = parameter["value"]
    # load network config
    network_config =  untangle.parse(network_config_path)
    # load featuremaps
    featuremaps = dd.io.load(featuremaps_path)
    # iterate over partitions of the accelerator
    featuremaps_done = []
    for partition in network_config.networkspec.partition:
        # get featuremaps
        input_featuremap    = ""
        output_featuremap   = ""
        for parameter in partition.parameter:
            if parameter["id"] == "input_featuremap":
                input_featuremap = parameter["value"]            
            if parameter["id"] == "output_featuremap":
                output_featuremap = parameter["value"]
        # add partition input featuremap to encoder spec
        if input_featuremap not in featuremaps_done:
            featuremaps_done.append(input_featuremap)
            layer = root.createElement("layer")
            layer.setAttribute("id", input_featuremap)
            # add info for encoding scheme
            if encoder == "baseline":
                add_baseline_parameters(root, layer, platform_info, featuremaps)
            if encoder == "bi":
                add_bi_parameters(root, layer, platform_info, featuremaps)
            if encoder == "def":
                add_def_parameters(root, layer, platform_info, featuremaps)
            if encoder == "rle":
                add_rle_parameters(root, layer, platform_info, featuremaps)
            if encoder == "huffman":
                add_huffman_parameters(root, layer, platform_info, featuremaps)
            if encoder == "pbm":
                add_pbm_parameters(root, layer, platform_info, featuremaps)
            # add layer to encoderspec
            encoderspec.appendChild(layer)
        # add partition output featuremap to encoder spec
        if output_featuremap not in featuremaps_done:
            featuremaps_done.append(output_featuremap)
            layer = root.createElement("layer")
            layer.setAttribute("id", output_featuremap)
            # add info for encoding scheme
            if encoder == "baseline":
                add_baseline_parameters(root, layer, platform_info, featuremaps)
            if encoder == "bi":
                add_bi_parameters(root, layer, platform_info, featuremaps)
            if encoder == "def":
                add_def_parameters(root, layer, platform_info, featuremaps)
            if encoder == "rle":
                add_rle_parameters(root, layer, platform_info, featuremaps)
            if encoder == "huffman":
                add_huffman_parameters(root, layer, platform_info, featuremaps)
            if encoder == "pbm":
                add_pbm_parameters(root, layer, platform_info, featuremaps)
            # add layer to encoderspec
            encoderspec.appendChild(layer)
    # save encoder config
    with open(output_path,"w") as f:
        f.write(root.toprettyxml(indent="\t"))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Encoder Config Generator Script")
    parser.add_argument("-e","--encoder",required=True, help="encoder name (def,bi,awr,abe,huffman,rle,pbm)")
    parser.add_argument("-a","--accelerator_config_path",required=True, help="accelerator config path (.xml)")
    parser.add_argument("-n","--network_config_path",required=True, help="network config path (.xml)")
    parser.add_argument("-f","--featuremaps_path",required=True, help="featuremaps path (.h5)")
    parser.add_argument("-o","--output_path",required=True, help="output path (.xml)")

    args = parser.parse_args()
    generate_encoder_config(
            args.encoder,
            args.accelerator_config_path,
            args.network_config_path,
            args.featuremaps_path,
            args.output_path
    ) 

