import re
import os
import json
import argparse

def parse_cacti_report(report_path):
    
    # regex expressions
    io_dynamic_and_phy      = re.compile("IO Dynamic Power \(mW\) = ([+-]?[0-9]*[.]?[0-9]+) PHY Power \(mW\) = ([+-]?[0-9]*[.]?[0-9]+)") 
    io_termination_and_bias = re.compile("IO Termination and Bias Power \(mW\) = ([+-]?[0-9]*[.]?[0-9]+)") 

    # power dictionary
    cacti_power = {
        "io_dynamic" : 0.0,
        "io_phy"     : 0.0,
        "io_bias"    : 0.0
    }

    # open file
    with open(report_path,"r") as f:
        # iterate over lines in file
        for line in f:
            # try to get matches
            if io_dynamic_and_phy.match(line):
                cacti_power["io_dynamic"] = float(io_dynamic_and_phy.match(line).group(1))
                cacti_power["io_phy"] = float(io_dynamic_and_phy.match(line).group(2))
            if io_termination_and_bias.match(line):
                cacti_power["io_bias"] = float(io_termination_and_bias.match(line).group(1))

    return cacti_power

def parse_dram_power_report(report_path):
    
    # regex expressions
    trace_length = re.compile("Total Trace Length \(clock cycles\): ([0-9]+)")
    trace_energy = re.compile("Total Trace Energy: ([+-]?[0-9]*[.]?[0-9]+)")
    trace_power  = re.compile("Average Power: ([+-]?[0-9]*[.]?[0-9]+)")

    # power dictionary
    dram_power = {
        "trace_length"  : 0,
        "trace_energy"  : 0.0,
        "trace_power"   : 0.0
    }

    # open file
    with open(report_path,"r") as f:
        # iterate over lines in file
        for line in f:
            # try to get matches
            if trace_length.match(line):
                dram_power["trace_length"] = int(trace_length.match(line).group(1))
            if trace_energy.match(line):
                dram_power["trace_energy"] = float(trace_energy.match(line).group(1))
            if trace_power.match(line):
                dram_power["trace_power"]  = float(trace_power.match(line).group(1))
    
    return dram_power

def add_power_to_report(output_path):
    with open(os.path.join(output_path,"report.json"),"r") as f:
        report = json.load(f)
    # iterate over folders in path
    for partition in os.listdir(output_path): 
        if not os.path.isdir(os.path.join(output_path,partition)):
            continue
        report[partition]["in"]["io"]   = parse_cacti_report(os.path.join(output_path,partition,"input_cacti.rpt"))
        report[partition]["in"]["dram"] = parse_dram_power_report(os.path.join(output_path,partition,"input_dram_power.rpt"))
        report[partition]["out"]["io"]   = parse_cacti_report(os.path.join(output_path,partition,"output_cacti.rpt"))
        report[partition]["out"]["dram"] = parse_dram_power_report(os.path.join(output_path,partition,"output_dram_power.rpt"))
    # save report to output path
    with open(os.path.join(output_path,"report.json"),"w") as f:
        json.dump(report,f,indent=4)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Add Power Measurements")
    parser.add_argument("-p","--path",required=True, help="path")

    args = parser.parse_args()
 
    add_power_to_report(args.path)
