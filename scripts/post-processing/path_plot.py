import json
from os.path import join
import math
import matplotlib.pyplot as plt
import sys
import numpy as np

def plot_path_xyz(data_dir):
    net_pose = open(join(data_dir, 'net', 's2d', 'egomotion.txt')).readlines()
    js = json.load(open(join(data_dir, 'data.json')))

    coords = [[0], [0], [0]]
    gt_coords = [[0], [0], [0]]

    mapping = {0: 'x', 1: 'y', 2: 'z'}

    last = None
    up_to = 3500
    # up_to = len(net_pose)

    for i in range(up_to):
        if i > 0:
            for k in mapping:
                if k != '0':
                    gt_coords[k].append(js[str(i)][mapping[k]])
                else:
                    gt_coords[k].append(js[str(i)][mapping[k]])
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

    for i in range(3):
        coords[i] = normalize(coords[i])
        gt_coords[i] = normalize(gt_coords[i])

    plot_axes(coords, gt_coords)
    # ax.plot(coords[0], coords[1], coords[2])
    plt.show()

def normalize(ar):
    ar = np.array(ar)
    mx = np.max(ar)
    mn = np.min(ar)
    for j in range(len(ar)):
        ar[j] = (ar[j]) / (mx - mn)
    return ar

def plot_axes(coords, gt_coords):
    fig = plt.figure(figsize=(10, 5))
    ax = fig.add_subplot(111)
    ax.set_xlabel('Frame Number')
    ax.set_ylabel('unit')

    plt.plot(-1 * np.array(coords[0]), marker='', color='pink', linewidth=2, label='ML_x')
    # plt.plot(np.array(coords[1]), marker='', color='blue', linewidth=2, label='ML_y')

    plt.plot(np.array(gt_coords[0]), marker='', color='red', linewidth=2, label='GT_x')
    # plt.plot(np.array(gt_coords[1]), marker='', color='green', linewidth=2, label='GT_y')
    # plt.plot(np.array(coords[2]), marker='', color='blue', linewidth=2, label='z')

    plt.draw()
    plt.legend()
    plt.show()


def imline_to_ar(im_line):
    splt = im_line.split(",")
    return [float(splt[i]) for i in range(3)]

if __name__ == '__main__':
    fig = plt.figure()
    script, path = sys.argv
    plot_path_xyz("pre-processed-data/" + path)