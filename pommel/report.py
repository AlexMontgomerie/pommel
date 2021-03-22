import re
import os
import json
import numpy as np

import pommel

class report:

    def __init__(self, report_path):

        # open report
        with open(report_path,"r") as f:
            self.report = json.load(f)
   
        # open config instances
        #self.network = pommel.network(self.report["network_path"])
        #self.accelerator = pommel.accelerator(self.report["accelerator_path"])
        self.memory = pommel.memory(self.report["memory_path"])

    def parse_cacti_report(self, cacti_report_path):
        # regex expressions
        io_dynamic_and_phy      = re.compile("IO Dynamic Power \(mW\) = ([+-]?[0-9]*[.]?[0-9]+) PHY Power \(mW\) = ([+-]?[0-9]*[.]?[0-9]+)") 
        io_termination_and_bias = re.compile("IO Termination and Bias Power \(mW\) = ([+-]?[0-9]*[.]?[0-9]+)") 

        # values
        io_dynamic = 0
        io_phy = 0
        io_bias = 0

        # open file
        with open(cacti_report_path,"r") as f:
            # iterate over lines in file
            for line in f:
                # try to get matches
                if io_dynamic_and_phy.match(line):
                    io_dynamic = float(io_dynamic_and_phy.match(line).group(1))
                    io_phy = float(io_dynamic_and_phy.match(line).group(2))
                if io_termination_and_bias.match(line):
                    io_bias = float(io_termination_and_bias.match(line).group(1))

        # return values
        return io_dynamic, io_phy, io_bias

    # add power reports
    def add_io_power(self, index, cacti_read_path, cacti_write_path, cacti_idle_path):

        # get all readings
        read_io_dynamic, read_io_phy, read_io_bias = self.parse_cacti_report(cacti_read_path)
        write_io_dynamic, write_io_phy, write_io_bias = self.parse_cacti_report(cacti_write_path)
        idle_io_dynamic, idle_io_phy, idle_io_bias = self.parse_cacti_report(cacti_idle_path)

        # get duty cycles
        read_duty_cycle = self.report["partitions"][index]["read_duty_cycle"]
        write_duty_cycle = self.report["partitions"][index]["write_duty_cycle"]
        idle_duty_cycle = self.report["partitions"][index]["idle_duty_cycle"]

        # update reports
        self.report["partitions"][index]["io"] = {}
        self.report["partitions"][index]["io"]["io_dynamic"] = read_io_dynamic + write_io_dynamic + idle_io_dynamic
        self.report["partitions"][index]["io"]["io_phy"] = read_duty_cycle*read_io_phy + write_duty_cycle*write_io_phy + idle_duty_cycle*idle_io_phy
        self.report["partitions"][index]["io"]["io_bias"] = read_io_bias + write_io_bias + idle_io_bias
    
    def parse_drampower_report(self, drampower_report_path):

        # regex expressions
        trace_length = re.compile("Total Trace Length \(clock cycles\): ([0-9]+)")
        trace_energy = re.compile("Total Trace Energy: ([+-]?[0-9]*[.]?[0-9]+)")
        trace_power  = re.compile("Average Power: ([+-]?[0-9]*[.]?[0-9]+)")

        # init values
        trace_length_val = 0
        trace_energy_val = 0
        trace_power_val = 0

        # open file
        with open(drampower_report_path,"r") as f:
            # iterate over lines in file
            for line in f:
                # try to get matches
                if trace_length.match(line):
                    trace_length_val = int(trace_length.match(line).group(1))
                if trace_energy.match(line):
                    trace_energy_val = float(trace_energy.match(line).group(1))
                if trace_power.match(line):
                    trace_power_val  = float(trace_power.match(line).group(1))
        
        # return values
        return trace_length_val, trace_energy_val, trace_power_val

    def add_dram_power(self, index, drampower_report_path, direction="in"):
        
        # get dram power report
        trace_length, trace_energy, trace_power = self.parse_drampower_report(drampower_report_path)

        # update report
        self.report["partitions"][index]["dram"] = {}
        self.report["partitions"][index]["dram"]["trace_length"] = trace_length
        self.report["partitions"][index]["dram"]["trace_energy"] = trace_energy*self.memory.chips
        self.report["partitions"][index]["dram"]["trace_power"] = trace_power*self.memory.chips

    def get_base_sequence(self, field):
        seq = []
        for i in self.report["partitions"]:
            seq.append(self.report["partitions"][i][field])
        return np.array(seq)

    def get_sequence(self, field, direction="in"):
        seq = []
        for i in self.report["partitions"]:
            seq.append(self.report["partitions"][i][direction][field])
        return np.array(seq)

    def get_io_sequence(self, field):
        seq = []
        for i in self.report["partitions"]:
            seq.append(self.report["partitions"][i]["io"][field])
        return np.array(seq)

    def get_dram_sequence(self, field):
        seq = []
        for i in self.report["partitions"]:
            seq.append(self.report["partitions"][i]["dram"][field])
        return np.array(seq)

    def get_total_io_power_sequence(self):
        io_phy      = self.get_io_sequence("io_phy")
        io_dynamic  = self.get_io_sequence("io_dynamic")
        io_bias     = self.get_io_sequence("io_bias")
        return io_phy + io_dynamic + io_bias

    def get_total_dram_power_sequence(self):
        return self.get_dram_sequence("trace_power")

    def get_average_activity(self, direction="in"):
        # get activity 
        activity = self.get_sequence("activity",direction)
        # get all samples
        samples = self.get_dram_sequence("trace_length")
        # return weighted average of power
        return np.sum(activity*samples)/np.sum(samples)

    def get_average_bandwidth(self):
        # get bandwidth
        bandwidth = self.get_sequence("bandwidth","in")
        # get all samples
        samples = self.get_dram_sequence("trace_length")
        # return weighted average of power
        return np.sum(bandwidth*samples)/np.sum(samples)
   
    def get_average_power(self):
        # get total power
        total_power = self.get_total_io_power_sequence() + self.get_total_dram_power_sequence()
        # get all samples
        samples = self.get_dram_sequence("trace_length")
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


