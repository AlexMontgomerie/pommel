import csv
import numpy as np
import xmltodict
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from sklearn.linear_model import LinearRegression
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


# load model
model = pommel.report("outputs/activity_bandwidth/report.json")

activity_model = model.get_sequence("data_activity", "in")
bandwidth_model = model.get_base_sequence("bandwidth")
power_model = model.get_total_power_sequence()

fig = plt.figure()
ax = Axes3D(fig)


ax.scatter(activity, bandwidth, power, label="actual")
ax.scatter(activity_model, bandwidth_model, power_model, label="model")
plt.legend()
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
print(reg.score(X,y))
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
print(reg.score(X,y))
print(reg.coef_)


