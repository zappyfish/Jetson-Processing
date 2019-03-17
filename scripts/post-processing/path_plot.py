import json
from os.path import join
import math
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import sys
import numpy as np

def plot_path_xyz(data_dir):
    net_pose = open(join(data_dir, 'net', 's2d', 'egomotion.txt')).readlines()

    coords = [[0], [0], [0]]

    last = None

    for i in range(len(net_pose)):
        line = net_pose[i]
        splt = line.strip().split(" ")
        im12 = splt[1]
        im23 = splt[2]
        nxt = imline_to_ar(im23)
        cur = imline_to_ar(im12)
        if last is None:
            for c in range(3):
                coords[c].append(cur[c] + coords[c][i])
        else:
            for c in range(3):
                coords[c].append(coords[c][i] + ((cur[c] + last[c]) / 2))
            last = nxt

    plot_axes(coords)

    ax.plot(coords[0], coords[1], coords[2])
    plt.show()

def plot_axes(coords):
    fig = plt.figure(figsize=(10, 5))
    ax = fig.add_subplot(111)
    ax.set_xlabel('Frame Number')
    ax.set_ylabel('unit')

    plt.plot(np.array(coords[0]), marker='', color='red', linewidth=2, label='x')
    plt.plot(np.array(coords[1]), marker='', color='green', linewidth=2, label='y')
    plt.plot(np.array(coords[2]), marker='', color='blue', linewidth=2, label='z')

    plt.draw()
    plt.legend()
    plt.show()


def imline_to_ar(im_line):
    splt = im_line.split(",")
    return [float(splt[i]) for i in range(3)]

if __name__ == '__main__':
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    script, path = sys.argv
    plot_path_xyz("pre-processed-data/" + path)