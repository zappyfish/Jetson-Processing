import json
from os.path import join
from math import sqrt
import numpy as np
import matplotlib.pyplot as plt
import sys

def plot_accels(data_dir):
    data = json.load(open(join(data_dir, 'data.json')))
    x = 'accel_x'
    y = 'accel_y'
    z = 'accel_z'
    mag = 'mag'

    pts = {x: [], y: [], z: [], mag: []}

    for i in range(len(data)):
        key = str(i)
        tot = 0
        for k in pts:
            if k is not mag:
                pts[k].append(data[key][k])
                tot += data[key][k] * data[key][k]
        pts[mag].append(sqrt(tot))

    generate_plot(pts[x], pts[y], pts[z], pts[mag], 'Filtered Acceleration Over Frames')

def generate_plot(x, y, z, mag, name):
    fig = plt.figure(figsize=(10, 5))
    ax = fig.add_subplot(111)
    ax.set_xlabel('Frame Number')
    ax.set_ylabel('m/s^2')
    ax.set_title(name + ': IMU Ground Truth vs. Camera Feed Prediction')
    plt.plot(np.array(x), marker='', color='red', linewidth=2, label='x')
    plt.plot(np.array(y), marker='', color='green', linewidth=2, label='y')
    plt.plot(np.array(z), marker='', color='blue', linewidth=2, label='z')
    plt.plot(np.array(mag), marker='', color='black', linewidth=2, label='mag')
    plt.draw()
    plt.legend()
    plt.show()

if __name__ == '__main__':
    script, path = sys.argv
    plot_accels("pre-processed-data/" + path)
