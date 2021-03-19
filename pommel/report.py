import re
import os
import json
import numpy as np

class report:

    def __init__(self, report_path):
        with open(report_path,"r") as f:
            self.report = json.load(f)
   
    # add power reports
    def add_io_power(self, index, cacti_report_path, direction="in"):
        
        # regex expressions
        io_dynamic_and_phy      = re.compile("IO Dynamic Power \(mW\) = ([+-]?[0-9]*[.]?[0-9]+) PHY Power \(mW\) = ([+-]?[0-9]*[.]?[0-9]+)") 
        io_termination_and_bias = re.compile("IO Termination and Bias Power \(mW\) = ([+-]?[0-9]*[.]?[0-9]+)") 

        # open file
        with open(cacti_report_path,"r") as f:
            # iterate over lines in file
            for line in f:
                # try to get matches
                if io_dynamic_and_phy.match(line):
                    self.report[index][direction]["io"]["io_dynamic"] = float(io_dynamic_and_phy.match(line).group(1))
                    self.report[index][direction]["io"]["io_phy"] = float(io_dynamic_and_phy.match(line).group(2))
                if io_termination_and_bias.match(line):
                    self.report[index][direction]["io"]["io_bias"] = float(io_termination_and_bias.match(line).group(1))

    def add_dram_power(self, index, drampower_report_path, direction="in"):
        
        # regex expressions
        trace_length = re.compile("Total Trace Length \(clock cycles\): ([0-9]+)")
        trace_energy = re.compile("Total Trace Energy: ([+-]?[0-9]*[.]?[0-9]+)")
        trace_power  = re.compile("Average Power: ([+-]?[0-9]*[.]?[0-9]+)")

        # open file
        with open(drampower_report_path,"r") as f:
            # iterate over lines in file
            for line in f:
                # try to get matches
                if trace_length.match(line):
                    self.report[index][direction]["dram"]["trace_length"] = int(trace_length.match(line).group(1))
                if trace_energy.match(line):
                    self.report[index][direction]["dram"]["trace_energy"] = float(trace_energy.match(line).group(1))
                if trace_power.match(line):
                    self.report[index][direction]["dram"]["trace_power"]  = float(trace_power.match(line).group(1))

    def get_base_sequence(self, field):
        seq = []
        for i in self.report:
            seq.append(self.report[i][field])
        return np.array(seq)

    def get_sequence(self, field, direction="in"):
        seq = []
        for i in self.report:
            seq.append(self.report[i][direction][field])
        return np.array(seq)

    def get_io_sequence(self, field, direction="in"):
        seq = []
        for i in self.report:
            seq.append(self.report[i][direction]["io"][field])
        return np.array(seq)

    def get_dram_sequence(self, field, direction="in"):
        seq = []
        for i in self.report:
            seq.append(self.report[i][direction]["dram"][field])
        return np.array(seq)

    def get_total_io_power_sequence(self, direction="in"):
        io_phy      = self.get_io_sequence("io_phy", direction)
        io_dynamic  = self.get_io_sequence("io_dynamic", direction)
        io_bias     = self.get_io_sequence("io_bias", direction)
        return io_phy + io_dynamic + io_bias

    def get_total_dram_power_sequence(self, direction="in"):
        return self.get_dram_sequence("trace_power", direction)

    def get_average_activity(self, direction="in"):
        # get activity 
        activity = self.get_sequence("activity",direction)
        # get all samples
        samples = self.get_dram_sequence("trace_length",direction)
        # return weighted average of power
        return np.sum(activity*samples)/np.sum(samples)

    def get_average_bandwidth(self, direction="in"):
        # get bandwidth
        bandwidth = self.get_sequence(report,"bandwidth",direction)
        # get all samples
        samples = self.get_dram_sequence("trace_length",direction)
        # return weighted average of power
        return np.sum(bandwidth*samples)/np.sum(samples)
   
    def get_average_power(self, direction="in"):
        # get total power
        total_power = self.get_total_io_power_sequence(direction) + self.get_total_dram_power_sequence(direction)
        # get all samples
        samples = self.get_dram_sequence("trace_length",direction)
        # return weighted average of power
        return np.sum(total_power*samples)/np.sum(samples)
   
    def get_latency(self, dram_freq):
        # get latencies in and out
        latency_seq_in  = self.get_dram_sequence("trace_length","in")/dram_freq
        latency_seq_out = self.get_dram_sequence("trace_length","out")/dram_freq
        return np.sum(np.maximum(latency_seq_in, latency_seq_out))

    def save_report(self, output_path):
        # save to json
        with open(output_path,"w") as f:
            json.dump(self.report,f,indent=4)


