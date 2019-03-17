import cv2
import os
import json
import sys
import matplotlib.pyplot as plt
import math
import numpy as np

def create_video(path, start_frame, end_frame):
    start_frame = int(start_frame)
    end_frame = int(end_frame)
    # pose_file = open(path + '/net/poses.txt', 'r').readlines()
    flight_json = json.load(open(path + '/data.json'))

    frame_one = cv2.imread(flight_json['0']['path'])
    start_time = int(flight_json['0']['time'])
    height, width, layers = frame_one.shape

    video = cv2.VideoWriter(path + '/playback.mp4', 0, 90, (width, height)) # 90 FPS 3x speed up

    font = cv2.FONT_HERSHEY_SIMPLEX

    imu_pts = []
    net_pts = []

    for i in range(start_frame, end_frame):
        data_pt = flight_json[str(i)]
        frame = cv2.imread(data_pt['path'])
        time = data_pt['time'] - start_time
        roll = data_pt['roll']
        pitch = data_pt['pitch']
        yaw = data_pt['yaw']
        if data_pt['target']:
            t_x = data_pt['target_x']
            t_y = data_pt['target_y']
            h, w, d = frame.shape
            w = w / 2
            h = h / 2
            cv2.circle(frame, (int(t_x + w), int(h - t_y)), 50, (0, 0, 255), cv2.FILLED)

        imu_pts.append(yaw)

        # tokens = pose_file[i].split(',')
        # net_pts.append(float(tokens[4]) * 180 / math.pi)
        text = "roll: %f, pitch: %f, yaw: %f" % (roll, pitch, yaw)
        cv2.putText(frame, text, (20, 50), font, 0.8, (0, 0, 255), 2, cv2.LINE_AA)

        video.write(frame)

    video.release()

    # fig = plt.figure(figsize=(10, 5))
    # ax = fig.add_subplot(111)
    # ax.set_xlabel('Frame Number')
    # ax.set_ylabel('Degrees')
    # ax.set_title('Calculated Pitch Using IMU vs. Camera Feed as Data Source')
    # plt.plot(imu_pts, marker='', color='red', linewidth=2, label='IMU')
    # plt.plot(net_pts, marker='', color='green', linewidth=2, label='Camera')
    # plt.draw()
    # plt.legend()
    # plt.show()

def euler_to_rot(theta):
        R_x = np.array([[1, 0, 0],
                        [0, math.cos(theta[0]), -math.sin(theta[0])],
                        [0, math.sin(theta[0]), math.cos(theta[0])]
                        ])

        R_y = np.array([[math.cos(theta[1]), 0, math.sin(theta[1])],
                        [0, 1, 0],
                        [-math.sin(theta[1]), 0, math.cos(theta[1])]
                        ])

        R_z = np.array([[math.cos(theta[2]), -math.sin(theta[2]), 0],
                        [math.sin(theta[2]), math.cos(theta[2]), 0],
                        [0, 0, 1]
                        ])

        R = np.dot(R_z, np.dot(R_y, R_x))

        return R

if __name__ == '__main__':
    script, path, start, end = sys.argv
    create_video("pre-processed-data/" + path, start, end)