import csv
import numpy as np
import xmltodict

import math
import pommel

# load mem power investigation results
activity = []
bandwidth = []
power = []
with open("outputs/mem_power_investigation.csv","r") as f:
    reader = csv.reader(f,delimiter=",")
    for row in reader:
        activity.append( float(int(row[1])&0xFFFFFFFF) / float(0xFFFFFFFF) )
        #bandwidth.append(256/float(row[0]))
        bandwidth.append(50000*256*4/(float(row[2])*1000000.0))
        power.append(float(row[3]))


# open network
network = pommel.network("config/network/template.xml")

# remove first layer
network.network["networkspec"]["partition"] = []

index = 0
"""
for i in np.arange(0.1, 1.1, 0.1):
    for j in np.arange(0, 110, 10):
        #print(i,j)
        #print("activity_{:0.2}0".format(j))
        bandwidth = max_memory_bw*i*0.5
        activity_path = f"activity_{j}"
        network.add_partition(index, activity_path, activity_path, bandwidth, bandwidth)
        index += 1
"""
for i in range(len(activity)):
    activity_path = "activity_{}".format(math.ceil(10*activity[i])*10)
    bw = 0.5*bandwidth[i]
    network.add_partition(index, activity_path, activity_path, bw, bw)
    index += 1

# save network
network.save_to_xml("config/network/activity_bandwidth_test.xml")
