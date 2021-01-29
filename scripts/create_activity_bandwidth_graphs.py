import matplotlib.pyplot as plt
import json
import numpy as np
from scipy.interpolate import griddata

# load report
with open("outputs/activity_bandwidth_ddr3/report.json", "r") as f:
#with open("outputs/activity_bandwidth_lpddr/report.json", "r") as f:
#with open("outputs/activity_bandwidth_wideio/report.json", "r") as f:
    report = json.load(f)

bandwidth = []
addr_activity = []
data_activity = []
io_power = []
dram_power = []
io_dynamic_power = []
io_bias_power = []

# get readings
for partition_index in report:
    partition = report[partition_index]
    bandwidth.append(partition["in"]["bandwidth"])
    data_activity.append(partition["in"]["data_activity"])
    addr_activity.append(partition["in"]["addr_activity"])
    io_power.append(partition["in"]["io"]["io_dynamic"]+partition["in"]["io"]["io_bias"])
    dram_power.append(partition["in"]["dram"]["trace_power"])
    io_dynamic_power.append(partition["in"]["io"]["io_dynamic"])
    io_bias_power.append(partition["in"]["io"]["io_bias"])

#bandwidth   = [0.1,0.5,1.0,5.0,10.0]
#activity     = [0.05,0.1,0.15]

print(bandwidth)
print(addr_activity)
print(data_activity)
print(io_power)
print(dram_power)

"""
fig = plt.figure()
ax = fig.gca(projection='3d')

ax.plot_surface(data_activity,bandwidth,io_power)
plt.show()
"""
fig = plt.figure()

ax = fig.add_subplot(111, projection='3d')

#ax.scatter(data_activity,bandwidth, io_bias_power,label="IO (bias)")
ax.scatter(data_activity,bandwidth, io_dynamic_power,label="IO (dynamic)")
ax.scatter(data_activity,bandwidth, dram_power,label="DRAM")
ax.set_xlabel("Activity")
ax.set_ylabel("Bandwidth (GB/s)")
ax.set_zlabel("Power (mW)")
ax.legend()
plt.show()

plt.scatter(bandwidth, io_bias_power,label="IO (bias)")
plt.scatter(bandwidth, io_dynamic_power,label="IO (dynamic)")
plt.scatter(bandwidth, dram_power,label="DRAM")
plt.legend()
plt.xlabel("Bandwidth (GB/s)")
plt.ylabel("Power (mW)")
#plt.scatter(data_activity, dram_power)
plt.show()
