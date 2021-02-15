import matplotlib.pyplot as plt
import json
import numpy as np
from scipy.interpolate import griddata

# load report
with open("outputs/scnn_mobilenet_v2_ddr3/report.json", "r") as f:
    report = json.load(f)

activity_in = []
activity_out = []
bandwidth_in = []
bandwidth_out = []
io_in_power = []
io_out_power = []
dram_in_power = []
dram_out_power = []

# get readings
for partition_index in report:
    partition = report[partition_index]
    io_in_power.append(partition["in"]["io"]["io_phy"]+partition["in"]["io"]["io_dynamic"]+partition["in"]["io"]["io_bias"])
    io_out_power.append(partition["out"]["io"]["io_phy"]+partition["out"]["io"]["io_dynamic"]+partition["out"]["io"]["io_bias"])
    dram_in_power.append(partition["in"]["dram"]["trace_power"])
    dram_out_power.append(partition["out"]["dram"]["trace_power"])
    activity_in.append(partition["in"]["data_activity"])
    activity_out.append(partition["out"]["data_activity"])
    bandwidth_in.append(partition["in"]["bandwidth"])
    bandwidth_out.append(partition["out"]["bandwidth"])


total_power_in = np.array(io_in_power)+np.array(dram_in_power)
total_power_out = np.array(io_out_power)+np.array(dram_out_power)

# plot bar graph for each partition
x = np.arange(len(report.keys()))
plt.bar(x-0.125, np.array(io_in_power)+np.array(dram_in_power), width=0.25, label="in")
plt.bar(x+0.125, np.array(io_out_power)+np.array(dram_out_power),width=0.25, label="out")
plt.xlabel("Partition")
plt.ylabel("Power (mW)")
plt.title("EYERISS Accelerator for AlexNet on DDR3")
plt.legend()
plt.grid()
#plt.bar(list(report.keys()), io_power)
plt.show()

# plot line graph of power for partitions
plt.plot(x, total_power_in, label="EYERISS")
plt.xlabel("Partition")
plt.ylabel("Power (mW)")
plt.title("AlexNet on DDR3")
plt.legend()
plt.grid()
#plt.bar(list(report.keys()), io_power)
plt.show()

# plot 3D activity, bandwidth
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(activity_in,bandwidth_in, total_power_in)
ax.scatter(activity_out,bandwidth_out, total_power_out)
ax.set_xlabel("Activity")
ax.set_ylabel("Bandwidth (GB/s)")
ax.set_zlabel("Power (mW)")
plt.show()

# plot against bandwidth
plt.scatter(bandwidth_in, total_power_in, label="in")
plt.scatter(bandwidth_out, total_power_out, label="out")
plt.legend()
plt.xlabel("Bandwidth (GB/s)")
plt.ylabel("Power (mW)")
plt.show()

print("average power (in): ", np.average(total_power_in))
print("average power (out): ", np.average(total_power_out))
print("average bandwidth (in): ", np.average(bandwidth_in))
print("average bandwidth (out): ", np.average(bandwidth_out))
print("average activity (in): ", np.average(activity_in))
print("average activity (out): ", np.average(activity_out))
