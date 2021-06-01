import xmltodict
import collections

class network:

    def __init__(self, network_path):

        # load network
        with open(network_path, "r") as f:
            self.network = xmltodict.parse(f.read())

        self.name = self.network["networkspec"]["@name"]

    # get fields
    def get_all_partitions(self):
        return range(len(self.network["networkspec"]["partition"]))

    def get_kernel_size(self, index):
        return int(self.network["networkspec"]["partition"][index]["parameter"][0]["@value"])

    def get_stride(self, index):
        return int(self.network["networkspec"]["partition"][index]["parameter"][1]["@value"])

    def get_input_featuremap(self, index):
        return self.network["networkspec"]["partition"][index]["parameter"][2]["@value"]

    def get_output_featuremap(self, index):
        return self.network["networkspec"]["partition"][index]["parameter"][3]["@value"]

    def get_bandwidth_in(self, index):
        return float(self.network["networkspec"]["partition"][index]["parameter"][4]["@value"])
    
    def get_bandwidth_out(self, index):
        return float(self.network["networkspec"]["partition"][index]["parameter"][5]["@value"])

    # update fields
    def update_kernel_size(self, index, val):
        self.network["networkspec"]["partition"][index]["parameter"][0]["@value"] = val

    def update_stride(self, index, val):
        self.network["networkspec"]["partition"][index]["parameter"][1]["@value"] = val

    def update_input_featuremap(self, index, val):
        self.network["networkspec"]["partition"][index]["parameter"][2]["@value"] = val

    def update_output_featuremap(self, index, val):
        self.network["networkspec"]["partition"][index]["parameter"][3]["@value"] = val

    def update_bandwidth_in(self, index, val):
        self.network["networkspec"]["partition"][index]["parameter"][4]["@value"] = val
    
    def update_bandwidth_out(self, index, val):
        self.network["networkspec"]["partition"][index]["parameter"][5]["@value"] = val

    def add_partition(self, index, fm_in, fm_out, bw_in, bw_out, k=1, s=1):
        # kernel size
        kernel_size = collections.OrderedDict({
            "@id"   : "kernel_size",
            "@type" : "int",
            "@value": k
        })
        # stride
        stride = collections.OrderedDict({
            "@id"   : "stride",
            "@type" : "int",
            "@value": s
        })
        # featuremap in
        featuremap_in = collections.OrderedDict({
            "@id"   : "input_featuremap",
            "@type" : "string",
            "@value": fm_in
        })
        # featuremap out 
        featuremap_out = collections.OrderedDict({
            "@id"   : "output_featuremap",
            "@type" : "string",
            "@value": fm_out
        })
        # bandwidth in 
        bandwidth_in = collections.OrderedDict({
            "@id"   : "bandwidth_in",
            "@type" : "float",
            "@value": bw_in
        })
        # bandwidth out 
        bandwidth_out = collections.OrderedDict({
            "@id"   : "bandwidth_out",
            "@type" : "float",
            "@value": bw_out
        })
        # add new partition
        partition = collections.OrderedDict({
            "@id" : index,
            "parameter" : [
                kernel_size,
                stride,
                featuremap_in,
                featuremap_out,
                bandwidth_in,
                bandwidth_out
            ]
        })
        # add partition
        self.network["networkspec"]["partition"].append(partition)

    #save to xml
    def save_to_xml(self, output_path):
        with open(output_path, "w") as f:
            f.write(xmltodict.unparse(self.network, pretty=True))

