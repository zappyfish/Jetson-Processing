import json
from os.path import join
import math
import matplotlib.pyplot as plt
import sys
import numpy as np
from kalman_algo import PoseKalmanFilter

def generate_plots(data_dir):
    net_pose = open(join(data_dir, 'net', 's2d', 'egomotion.txt')).readlines()
    # slam_pose = open(join(data_dir, 'slam', 'poses.txt')).readlines()
    slam = False
    imu_pose = json.load(open(join(data_dir, 'data.json')))
    pkf = PoseKalmanFilter(data_dir, 0.01)
    r_x_n = []
    r_y_n = []
    r_z_n = []

    r_x_s = []
    r_y_s = []
    r_z_s = []

    r_x_raw = []
    r_z_raw = []

    r_x_k = []
    r_z_k = []

    r_x_i = []
    r_y_i = []
    r_z_i = []
    initial = imu_pose['0']
    offset = 0
    for i in range(len(net_pose)):

        data_point = imu_pose[str(i - offset)]
        r_y_i.append((data_point['yaw'] - initial['yaw']) * math.pi / 180)
        r_z_i.append((data_point['roll'] - initial['roll']) * math.pi / 180)
        r_x_i.append((data_point['pitch'] - initial['pitch']) * math.pi / 180)

        if i >= 1:
            line = net_pose[i]
            s = line.strip().split(" ")[1].split(",")
            ind = len(r_x_n) - 1
            r_x_n.append(float(s[3]) + r_x_n[ind])
            r_y_n.append(float(s[4]) + r_y_n[ind])
            r_z_n.append(float(s[5]) + r_z_n[ind])

            pkf_vals, raw_imu = pkf.kalman_step()

            r_z_k.append(pkf_vals['roll'])
            r_x_k.append(pkf_vals['pitch'])

            r_z_raw.append(raw_imu['roll'])
            r_x_raw.append(raw_imu['pitch'])
        else:
            r_x_n.append(r_x_i[0])
            r_y_n.append(r_y_i[0])
            r_z_n.append(r_z_i[0])

            pkf_initial = pkf.value
            r_z_k.append(pkf_initial['roll'])
            r_x_k.append(pkf_initial['pitch'])

            r_z_raw.append(pkf_initial['roll'])
            r_x_raw.append(pkf_initial['pitch'])

        if slam:
            if i < len(slam_pose):
                slam_line = slam_pose[i].split(",")
                r_x_s.append(float(slam_line[0]))
                r_y_s.append(float(slam_line[1]))
                r_z_s.append(float(slam_line[2]))


    max_frame = 0
    max = 0
    for i, x, in enumerate(r_x_k):
        if x > max:
            max_frame = i
            max = x

    print("max frame: " + str(max_frame))

    corr_x = corr(r_x_n, r_x_i)
    corr_y = corr(r_y_n, r_y_i)
    corr_z = corr(r_z_n, r_z_i)

    corr_kf_x = corr(r_x_i, r_x_k)
    corr_kf_z = corr(r_x_i, r_z_k)

    corr_raw_x = corr(r_x_i, r_x_raw)
    corr_raw_z = corr(r_z_i, r_z_raw)

    # corr_slam_x = corr(r_x_s, r_x_i)
    # corr_slam_y = corr(r_y_s, r_y_i)
    # corr_slam_z = corr(r_z_s, r_z_i)

    # r_x_i = get_normalized(r_x_i)
    # r_x_n = get_normalized(r_x_n)

    print("ML correlation x: %f, correlation y: %f, correlation z: %f" % (corr_x, corr_y, corr_z))
    # print("SLAM correlation x: %f, correlation y: %f, correlation z: %f" % (corr_slam_x, corr_slam_y, corr_slam_z))
    print("KF correlation x: %f, correlation z: %f" % (corr_kf_x, corr_kf_z))
    # print("Raw correlation x: %f, correlation z: %f" % (corr_raw_x, corr_raw_z))


    kf_x_err,  kf_x_err_avg = error(r_x_k, r_x_i)
    kf_z_err, kf_z_err_avg = error(r_z_k, r_z_i)
    print("KF Max Error x: %f, z: %f" % (kf_x_err, kf_z_err))
    print("KF Avg Error x: %f, z: %f" % (kf_x_err_avg, kf_z_err_avg))

    # raw_x_err, raw_x_err_avg = error(r_x_raw, r_x_i)
    # raw_z_err, raw_z_err_avg = error(r_z_raw, r_z_i)
    # print("Raw Max Error x: %f, z: %f" % (raw_x_err, raw_z_err))
    # print("Raw Avg Error x: %f, z: %f" % (raw_x_err_avg, raw_z_err_avg))


    r_x_i = get_normalized(r_x_i)
    r_y_i = get_normalized(r_y_i)
    r_z_i = get_normalized(r_z_i)

    r_x_n = get_normalized(r_x_n)
    r_y_n = get_normalized(r_y_n)
    r_z_n = get_normalized(r_z_n)

    generate_plot(r_x_i, r_x_n, 'Pitch', r_x_k)
    generate_plot(r_y_i, r_y_n, 'Yaw')
    generate_plot(r_z_i, r_z_n, 'Roll', r_z_k)

    plot_error(r_x_i, r_x_n, 'Pitch')
    plot_error(r_y_i, r_y_n, 'Yaw')
    plot_error(r_z_i, r_z_n, 'Roll')


def generate_plot(imu, net, name, kf=None, raw_imu=None):
    fig = plt.figure(figsize=(10, 5))
    ax = fig.add_subplot(111)
    ax.set_xlabel('Frame Number')
    ax.set_ylabel('Degrees')
    ax.set_title(name + ': IMU Ground Truth vs. Camera Feed Prediction')
    plt.plot(np.array(imu) * 180 / math.pi, marker='', color='red', linewidth=2, label='IMU Ground Truth')
    plt.plot(np.array(net) * 180 / math.pi, marker='', color='green', linewidth=2, label='Net Prediction')
    if kf is not None:
        plt.plot(np.array(kf) * 180 / math.pi, marker='', color='blue', linewidth=2, label='Net + LPF + KF Prediction')
    if raw_imu is not None:
        plt.plot(np.array(raw_imu) * 180 / math.pi, marker='', color='black', linewidth=2, label='LPF Prediction')
    plt.draw()
    plt.legend()
    plt.show()

def plot_error(imu, net, name, slam=None):
    fig = plt.figure(figsize=(10, 5))
    ax = fig.add_subplot(111)
    ax.set_xlabel('Frame Number')
    ax.set_ylabel('% Error')
    ax.set_title(name + ' Absolute Error from Predictions')
    lbl = name + ' Absolute Error Between Neural Net and IMU'
    error = [100 * abs(imu[i] - net[i]) / (1 + abs(imu[i])) for i in range(len(net))]
    plt.plot(error, marker='', color='red', linewidth=2, label=lbl)
    if slam is not None:
        error_slam = [100 * abs(imu[i] - slam[i]) / (1 + abs(imu[i])) for i in range(len(imu))]
        lbl_slam = name + ' Absolute Error Between SLAM and IMU'
        plt.plot(error_slam, marker='', color='blue', linewidth=2, label=lbl_slam)
    plt.draw()
    plt.legend()
    plt.show()


def corr(A,B):
    a_avg = np.average(A)
    b_avg = np.average(B)

    correlation = 0

    for pt in range(len(A)):
        correlation += (A[pt] - a_avg) * (B[pt] - b_avg)

    correlation = correlation / (len(A) * np.std(A) * np.std(B))

    # Finally get corr coeff
    return correlation

def get_normalized(x):
    mn = np.min(x)
    mx = np.max(x)
    for i in range(len(x)):
        x[i] = 2 * ((x[i] - mn) / (mx - mn)) - 1
    return x

def error(a, b):
    max_error = 0
    avg_error = 0
    for i in range(len(a)):
        max_error = max(max_error, abs(a[i] - b[i]))
        avg_error += abs(a[i] - b[i])
    return max_error * 180 / math.pi,((avg_error * 180) / (len(a) * math.pi))

if __name__ == '__main__':
    script, path = sys.argv
    generate_plots("pre-processed-data/" + path)
