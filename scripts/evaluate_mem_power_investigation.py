import csv
import numpy as np
import xmltodict
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from sklearn.linear_model import LinearRegression
import math

import pommel

import json
import matplotlib.style
import matplotlib as mpl
from cycler import cycler

# setup plotting parameters
mpl.rcParams['axes.prop_cycle'] = cycler(color='bgrcmyk')

mpl.rcParams['grid.color'] = 'k'
mpl.rcParams['grid.linestyle'] = ':'
mpl.rcParams['grid.linewidth'] = 0.5

mpl.rcParams['figure.figsize'] = [8.0, 6.0]
mpl.rcParams['figure.dpi'] = 80
mpl.rcParams['savefig.dpi'] = 100

mpl.rcParams['font.size'] = 15 
mpl.rcParams['legend.fontsize'] = 'small'
mpl.rcParams['figure.titlesize'] = 'small'

mpl.rcParams['legend.fancybox'] = False
#mpl.rcParams['legend.fontsize'] = 'large'
mpl.rcParams['legend.framealpha'] = None
mpl.rcParams['legend.edgecolor'] = 'inherit'

mpl.rcParams['patch.force_edgecolor'] = True
mpl.rcParams['patch.facecolor'] = 'b'

mpl.rcParams['lines.linewidth'] = 1.0
mpl.rcParams['lines.dashed_pattern'] = [6, 6]
mpl.rcParams['lines.dashdot_pattern'] = [3, 5, 1, 5]
mpl.rcParams['lines.dotted_pattern'] = [1, 3]
mpl.rcParams['lines.scale_dashes'] = False

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


# load model
model = pommel.report("outputs/activity_bandwidth/report.json")

activity_model = model.get_sequence("data_activity", "in")
bandwidth_model = model.get_base_sequence("bandwidth")
power_model = model.get_total_power_sequence()

"""
fig = plt.figure()
ax = Axes3D(fig)


ax.scatter(activity, bandwidth, power, label="actual")
ax.scatter(activity_model, bandwidth_model, power_model, label="model")
plt.legend()
plt.show()
"""

# activity of 0.1
activity_010    = []
bandwidth_010   = []
power_010       = []
for i in range(10,20):
    activity_010.append(activity[i])
    bandwidth_010.append(bandwidth[i])
    power_010.append(power[i])
 
activity_model_010    = []
bandwidth_model_010   = []
power_model_010       = []
for i in range(10):
    activity_model_010.append(activity_model[i])
    bandwidth_model_010.append(bandwidth_model[i])
    power_model_010.append(power_model[i])

activity_090    = []
bandwidth_090   = []
power_090       = []
for i in range(80,90):
    activity_090.append(activity[i])
    bandwidth_090.append(bandwidth[i])
    power_090.append(power[i])
 
activity_model_090    = []
bandwidth_model_090   = []
power_model_090       = []
for i in range(80,90):
    activity_model_090.append(activity_model[i])
    bandwidth_model_090.append(bandwidth_model[i])
    power_model_090.append(power_model[i])

print(activity_model_090)

plt.plot(bandwidth_010, power_010,color='b',label="readings (activity=0.1)")
plt.plot(bandwidth_090, power_090,color='r',label="readings (activity=0.9)")
plt.plot(bandwidth_model_010, power_model_010,color='b',linestyle="dashed",label="model (activity=0.1)")
plt.plot(bandwidth_model_090, power_model_090,color='r',linestyle="dashed",label="model (activity=0.9)")
plt.ylim(bottom=0)
plt.xlim([min(bandwidth_010), max(bandwidth_010)])
plt.xlabel("Bandwidth (GB/s)")
plt.ylabel("Average Power (mW)")

plt.grid()
plt.legend(loc="lower right")
plt.show()
X = []
y = []

for i in range(len(activity)):
    X.append([activity[i]*bandwidth[i], bandwidth[i]])
    y.append(power[i])

X = np.array(X)
y = np.array(y)

reg = LinearRegression().fit(X,y)
print(reg.intercept_)
#print(reg.score(X,y))
print(reg.coef_)

X = []
y = []

for i in range(len(activity)):
    X.append([activity_model[i]*bandwidth_model[i], bandwidth_model[i]])
    y.append(power_model[i])

X = np.array(X)
y = np.array(y)

reg = LinearRegression().fit(X,y)
print(reg.intercept_)
#print(reg.score(X,y))
print(reg.coef_)


