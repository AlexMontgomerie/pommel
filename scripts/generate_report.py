import argparse
import os

import pommel

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Add Power Measurements")
    parser.add_argument("-p","--path",required=True, help="path")

    args = parser.parse_args()

    # open report instance
    report = pommel.report( os.path.join(args.path,"report.json") )
    
    # iterate over partitions in report
    for partition in report.report["partitions"]:

        # check partition path exists
        if not os.path.isdir(os.path.join(args.path,partition)):
            print("could not find {} outputs".format(partition))
            raise FileNotFoundError

        #if partition == "layer_wise":
        # continue

        # get cacti file paths
        cacti_read_path  = os.path.join(args.path,partition,"read_cacti.rpt")
        cacti_write_path = os.path.join(args.path,partition,"write_cacti.rpt")
        cacti_idle_path  = os.path.join(args.path,partition,"idle_cacti.rpt")
        report.add_io_power(partition, cacti_read_path, cacti_write_path, cacti_idle_path)

        # get dram report
        dram_path = os.path.join(args.path,partition,"dram_power.rpt")
        report.add_dram_power(partition, dram_path)

    # save report to output path
    report.save_report( os.path.join(args.path,"report.json") )

