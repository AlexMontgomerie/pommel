import json
import numpy as np

def get_sequence(report, field, direction="in"):
    seq = []
    for i in report:
        seq.append(report[i][direction][field])
    return np.array(seq)

def get_io_sequence(report, field, direction="in"):
    seq = []
    for i in report:
        seq.append(report[i][direction]["io"][field])
    return np.array(seq)

def get_dram_sequence(report, field, direction="in"):
    seq = []
    for i in report:
        seq.append(report[i][direction]["dram"][field])
    return np.array(seq)

def get_total_io_power_sequence(report, direction="in"):
    io_phy      = get_io_sequence(report,"io_phy", direction)
    io_dynamic  = get_io_sequence(report,"io_dynamic", direction)
    io_bias     = get_io_sequence(report,"io_bias", direction)
    return io_phy + io_dynamic + io_bias

def get_total_dram_power_sequence(report, direction="in"):
    return get_dram_sequence(report,"trace_power", direction)

def get_average_activity(report, direction="in"):
    # get activity 
    activity = get_sequence(report,"activity",direction)
    # get all samples
    samples = get_sequence(report,"samples",direction)
    # return weighted average of power
    return np.sum(activity*samples)/np.sum(samples)

def get_average_bandwidth(report, direction="in"):
    # get bandwidth
    bandwidth = get_sequence(report,"bandwidth",direction)
    # get all samples
    samples = get_sequence(report,"samples",direction)
    # return weighted average of power
    return np.sum(bandwidth*samples)/np.sum(samples)

def get_average_power(report, direction="in"):
    # get total power
    total_power = get_total_io_power_sequence(report,direction) +  get_total_dram_power_sequence(report,direction)
    # get all samples
    samples = get_sequence(report,"samples",direction)
    # return weighted average of power
    return np.sum(total_power*samples)/np.sum(samples)
