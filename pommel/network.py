import xmltodict

class network:

    def __init__(self, network_path):

        # load network
        with open(network_path, "r") as f:
            self.network = xmltodict.parse(f.read())
    
    # get fields
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

    #save to xml
    def save_to_xml(self, output_path):
        with open(output_path, "w") as f:
            f.write(xmltodict.unparse(self.network, pretty=True))

