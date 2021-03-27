import deepdish as dd
import numpy as np

class featuremap:

    def __init__(self, featuremap_path):
        # load file
        self.featuremaps = dd.io.load(featuremap_path)
        # get all dimensions
        self.dimensions = {}
        for layer in self.featuremaps["layers"]:
            self.dimensions[layer] = self.featuremaps[layer].shape
        self.dimensions["data"] = self.featuremaps["data"].shape
        self.dimensions["output"] = self.featuremaps["output"].shape

    def get_channels(self, fm):
        return self.dimensions[fm][1]
 
    def get_rows(self, fm):
        if len(self.dimensions[fm]) > 2:
            return self.dimensions[fm][2]
        else: 
            return 1
        
    def get_cols(self, fm):
        if len(self.dimensions[fm]) > 2:
            return self.dimensions[fm][3]
        else: 
            return 1
